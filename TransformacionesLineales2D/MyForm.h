#pragma once

#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

namespace TransformacionesLineales2D {
    using namespace System::Drawing::Imaging;
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    struct Punto2D {
        double x, y;
    };

    // Estructura nativa para almacenar los pasos intermedios
    struct PasoTransformacion {
        std::string descripcion;
        std::vector<Punto2D> verticesEstado;
    };

    public ref class MyForm : public System::Windows::Forms::Form
    {
    private:
        // Punteros nativos
        std::vector<Punto2D>* verticesOriginales;
        std::vector<Punto2D>* verticesTransformados;
        std::vector<PasoTransformacion>* historialPasos;
        int contadorPuntos;

        // Imagen de Fondo Fija
        System::Drawing::Image^ imagenFondo;

        // Componentes GUI
        System::Windows::Forms::ComboBox^ cmbObjetos;
        System::Windows::Forms::Button^ btnCargar;
        System::Windows::Forms::Button^ btnSpawnAleatorio;
        System::Windows::Forms::Button^ btnTransformar;
        System::Windows::Forms::Button^ btnReiniciarTransformaciones;
        System::Windows::Forms::Button^ btnPoligonoAleatorio;
        System::Windows::Forms::Button^ btnCerrar;
        System::Windows::Forms::Panel^ canvasBox;

        System::Windows::Forms::TextBox^ txtCoordX;
        System::Windows::Forms::TextBox^ txtCoordY;
        System::Windows::Forms::Button^ btnAgregarPunto;
        System::Windows::Forms::Button^ btnLimpiarPuntos;
        System::Windows::Forms::Label^ lblCoordX;
        System::Windows::Forms::Label^ lblCoordY;
        System::Windows::Forms::Label^ lblContadorPuntos;

        System::Windows::Forms::TextBox^ txtTx;
        System::Windows::Forms::TextBox^ txtTy;
        System::Windows::Forms::TextBox^ txtSx;
        System::Windows::Forms::TextBox^ txtSy;
        System::Windows::Forms::TextBox^ txtAngulo;
        System::Windows::Forms::TextBox^ txtShx;
        System::Windows::Forms::TextBox^ txtShy;

        System::Windows::Forms::CheckBox^ chkReflejoX;
        System::Windows::Forms::CheckBox^ chkReflejoY;

        System::Windows::Forms::Label^ lblTx;
        System::Windows::Forms::Label^ lblTy;
        System::Windows::Forms::Label^ lblSx;
        System::Windows::Forms::Label^ lblSy;
        System::Windows::Forms::Label^ lblAngulo;
        System::Windows::Forms::Label^ lblShx;
        System::Windows::Forms::Label^ lblShy;

        System::Windows::Forms::ListBox^ lstPasos;
        System::Windows::Forms::Label^ lblTituloPasos;

    public:
        MyForm(void)
        {
            InitializeComponent();
            srand((unsigned int)time(NULL));

            // =========================================================================
            // CONFIGURACIÓN DE LA IMAGEN DE FONDO ESTÁTICA
            // Cambia "fondo.png" por el nombre de tu archivo de imagen
            // =========================================================================
            String^ nombreImagenFondo = L"wha.png";

            try {
                imagenFondo = Image::FromFile(nombreImagenFondo);
            }
            catch (...) {
                // Si la imagen no se encuentra en el directorio, se asigna nullptr para no romper el programa
                imagenFondo = nullptr;
            }

            // Reserva de memoria nativa
            verticesOriginales = new std::vector<Punto2D>();
            verticesTransformados = new std::vector<Punto2D>();
            historialPasos = new std::vector<PasoTransformacion>();
            contadorPuntos = 0;
        }

    protected:
        // Destructor: Buenas prácticas para liberación de memoria manual
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }

            // Liberar la imagen administrada si existe
            if (imagenFondo != nullptr) {
                delete imagenFondo;
                imagenFondo = nullptr;
            }

            // Liberar punteros a la memoria nativa
            if (verticesOriginales != nullptr) {
                delete verticesOriginales;
                verticesOriginales = nullptr;
            }
            if (verticesTransformados != nullptr) {
                delete verticesTransformados;
                verticesTransformados = nullptr;
            }
            if (historialPasos != nullptr) {
                delete historialPasos;
                historialPasos = nullptr;
            }
        }

    private:
        System::ComponentModel::Container^ components;

        // Conversión auxiliar entre System::String^ y std::string
        std::string SystemToStdString(System::String^ s) {
            using namespace System::Runtime::InteropServices;
            const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
            std::string os(chars);
            Marshal::FreeHGlobal(IntPtr((void*)chars));
            return os;
        }

        void InitializeComponent(void)
        {
            this->cmbObjetos = (gcnew System::Windows::Forms::ComboBox());
            this->btnCargar = (gcnew System::Windows::Forms::Button());
            this->btnSpawnAleatorio = (gcnew System::Windows::Forms::Button());
            this->btnPoligonoAleatorio = (gcnew System::Windows::Forms::Button());
            this->btnTransformar = (gcnew System::Windows::Forms::Button());
            this->btnReiniciarTransformaciones = (gcnew System::Windows::Forms::Button());
            this->btnCerrar = (gcnew System::Windows::Forms::Button());
            this->canvasBox = (gcnew System::Windows::Forms::Panel());
            this->txtCoordX = (gcnew System::Windows::Forms::TextBox());
            this->txtCoordY = (gcnew System::Windows::Forms::TextBox());
            this->btnAgregarPunto = (gcnew System::Windows::Forms::Button());
            this->btnLimpiarPuntos = (gcnew System::Windows::Forms::Button());
            this->lblCoordX = (gcnew System::Windows::Forms::Label());
            this->lblCoordY = (gcnew System::Windows::Forms::Label());
            this->lblContadorPuntos = (gcnew System::Windows::Forms::Label());
            this->txtTx = (gcnew System::Windows::Forms::TextBox());
            this->txtTy = (gcnew System::Windows::Forms::TextBox());
            this->txtSx = (gcnew System::Windows::Forms::TextBox());
            this->txtSy = (gcnew System::Windows::Forms::TextBox());
            this->txtAngulo = (gcnew System::Windows::Forms::TextBox());
            this->txtShx = (gcnew System::Windows::Forms::TextBox());
            this->txtShy = (gcnew System::Windows::Forms::TextBox());
            this->chkReflejoX = (gcnew System::Windows::Forms::CheckBox());
            this->chkReflejoY = (gcnew System::Windows::Forms::CheckBox());
            this->lblTx = (gcnew System::Windows::Forms::Label());
            this->lblTy = (gcnew System::Windows::Forms::Label());
            this->lblSx = (gcnew System::Windows::Forms::Label());
            this->lblSy = (gcnew System::Windows::Forms::Label());
            this->lblAngulo = (gcnew System::Windows::Forms::Label());
            this->lblShx = (gcnew System::Windows::Forms::Label());
            this->lblShy = (gcnew System::Windows::Forms::Label());
            this->lstPasos = (gcnew System::Windows::Forms::ListBox());
            this->lblTituloPasos = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();

            // cmbObjetos
            this->cmbObjetos->FormattingEnabled = true;
            this->cmbObjetos->Items->AddRange(gcnew cli::array< System::Object^  >(5) { L"Línea (0,0)", L"Casa", L"Auto", L"Perro", L"Personalizado" });
            this->cmbObjetos->Location = System::Drawing::Point(20, 20);
            this->cmbObjetos->Name = L"cmbObjetos";
            this->cmbObjetos->Size = System::Drawing::Size(230, 21);

            // btnCargar
            this->btnCargar->Location = System::Drawing::Point(20, 50);
            this->btnCargar->Name = L"btnCargar";
            this->btnCargar->Size = System::Drawing::Size(110, 25);
            this->btnCargar->Text = L"Cargar Objeto";
            this->btnCargar->Click += gcnew System::EventHandler(this, &MyForm::btnCargar_Click);

            // btnSpawnAleatorio
            this->btnSpawnAleatorio->Location = System::Drawing::Point(140, 50);
            this->btnSpawnAleatorio->Name = L"btnSpawnAleatorio";
            this->btnSpawnAleatorio->Size = System::Drawing::Size(110, 25);
            this->btnSpawnAleatorio->Text = L"Spawn Aleatorio";
            this->btnSpawnAleatorio->Click += gcnew System::EventHandler(this, &MyForm::btnSpawnAleatorio_Click);

            // btnPoligonoAleatorio
            this->btnPoligonoAleatorio->Location = System::Drawing::Point(20, 165);
            this->btnPoligonoAleatorio->Name = L"btnPoligonoAleatorio";
            this->btnPoligonoAleatorio->Size = System::Drawing::Size(230, 25);
            this->btnPoligonoAleatorio->Text = L"Generar Puntos Aleatorios";
            this->btnPoligonoAleatorio->Click += gcnew System::EventHandler(this, &MyForm::btnPoligonoAleatorio_Click);

            // chkReflejoX
            this->chkReflejoX->AutoSize = true;
            this->chkReflejoX->Location = System::Drawing::Point(20, 420);
            this->chkReflejoX->Name = L"chkReflejoX";
            this->chkReflejoX->Size = System::Drawing::Size(100, 17);
            this->chkReflejoX->Text = L"Reflejo en Eje X";

            // chkReflejoY
            this->chkReflejoY->AutoSize = true;
            this->chkReflejoY->Location = System::Drawing::Point(130, 420);
            this->chkReflejoY->Name = L"chkReflejoY";
            this->chkReflejoY->Size = System::Drawing::Size(100, 17);
            this->chkReflejoY->Text = L"Reflejo en Eje Y";

            // btnTransformar
            this->btnTransformar->Location = System::Drawing::Point(20, 445);
            this->btnTransformar->Name = L"btnTransformar";
            this->btnTransformar->Size = System::Drawing::Size(110, 30);
            this->btnTransformar->Text = L"Transformar";
            this->btnTransformar->Click += gcnew System::EventHandler(this, &MyForm::btnTransformar_Click);

            // btnReiniciarTransformaciones
            this->btnReiniciarTransformaciones->Location = System::Drawing::Point(140, 445);
            this->btnReiniciarTransformaciones->Name = L"btnReiniciarTransformaciones";
            this->btnReiniciarTransformaciones->Size = System::Drawing::Size(110, 30);
            this->btnReiniciarTransformaciones->Text = L"Reiniciar Valores";
            this->btnReiniciarTransformaciones->Click += gcnew System::EventHandler(this, &MyForm::btnReiniciarTransformaciones_Click);

            // lblTituloPasos
            this->lblTituloPasos->AutoSize = true;
            this->lblTituloPasos->Location = System::Drawing::Point(20, 482);
            this->lblTituloPasos->Name = L"lblTituloPasos";
            this->lblTituloPasos->Size = System::Drawing::Size(185, 13);
            this->lblTituloPasos->Text = L"Pasos del proceso (Haz click para ver):";

            // lstPasos
            this->lstPasos->FormattingEnabled = true;
            this->lstPasos->HorizontalScrollbar = true;
            this->lstPasos->Location = System::Drawing::Point(20, 498);
            this->lstPasos->Name = L"lstPasos";
            this->lstPasos->Size = System::Drawing::Size(230, 95);
            this->lstPasos->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::lstPasos_SelectedIndexChanged);

            // btnCerrar
            this->btnCerrar->BackColor = System::Drawing::Color::LightCoral;
            this->btnCerrar->Location = System::Drawing::Point(20, 605);
            this->btnCerrar->Name = L"btnCerrar";
            this->btnCerrar->Size = System::Drawing::Size(230, 35);
            this->btnCerrar->Text = L"Cerrar Aplicación";
            this->btnCerrar->UseVisualStyleBackColor = false;
            this->btnCerrar->Click += gcnew System::EventHandler(this, &MyForm::btnCerrar_Click);

            // canvasBox
            this->canvasBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
                | System::Windows::Forms::AnchorStyles::Left)
                | System::Windows::Forms::AnchorStyles::Right));
            this->canvasBox->BackColor = System::Drawing::Color::White;
            this->canvasBox->Location = System::Drawing::Point(280, 20);
            this->canvasBox->Name = L"canvasBox";
            this->canvasBox->Size = System::Drawing::Size(560, 620);
            this->canvasBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::canvasBox_Paint);
            this->canvasBox->Resize += gcnew System::EventHandler(this, &MyForm::canvasBox_Resize);

            // txtCoordX
            this->txtCoordX->Location = System::Drawing::Point(40, 87);
            this->txtCoordX->Size = System::Drawing::Size(40, 20);
            this->txtCoordX->Text = L"0";

            // txtCoordY
            this->txtCoordY->Location = System::Drawing::Point(105, 87);
            this->txtCoordY->Size = System::Drawing::Size(40, 20);
            this->txtCoordY->Text = L"0";

            // btnAgregarPunto
            this->btnAgregarPunto->Location = System::Drawing::Point(150, 85);
            this->btnAgregarPunto->Size = System::Drawing::Size(100, 25);
            this->btnAgregarPunto->Text = L"Agregar Punto";
            this->btnAgregarPunto->Click += gcnew System::EventHandler(this, &MyForm::btnAgregarPunto_Click);

            // btnLimpiarPuntos
            this->btnLimpiarPuntos->Location = System::Drawing::Point(20, 135);
            this->btnLimpiarPuntos->Size = System::Drawing::Size(230, 25);
            this->btnLimpiarPuntos->Text = L"Limpiar Coordenadas Manuales";
            this->btnLimpiarPuntos->Click += gcnew System::EventHandler(this, &MyForm::btnLimpiarPuntos_Click);

            // lblCoordX
            this->lblCoordX->AutoSize = true;
            this->lblCoordX->Location = System::Drawing::Point(20, 90);
            this->lblCoordX->Text = L"X:";

            // lblCoordY
            this->lblCoordY->AutoSize = true;
            this->lblCoordY->Location = System::Drawing::Point(85, 90);
            this->lblCoordY->Text = L"Y:";

            // lblContadorPuntos
            this->lblContadorPuntos->AutoSize = true;
            this->lblContadorPuntos->Location = System::Drawing::Point(20, 115);
            this->lblContadorPuntos->Text = L"Puntos agregados: 0";

            // Textboxes y labels
            this->txtTx->Location = System::Drawing::Point(150, 207); this->txtTx->Size = System::Drawing::Size(100, 20); this->txtTx->Text = L"0";
            this->txtTy->Location = System::Drawing::Point(150, 237); this->txtTy->Size = System::Drawing::Size(100, 20); this->txtTy->Text = L"0";
            this->txtSx->Location = System::Drawing::Point(150, 267); this->txtSx->Size = System::Drawing::Size(100, 20); this->txtSx->Text = L"1";
            this->txtSy->Location = System::Drawing::Point(150, 297); this->txtSy->Size = System::Drawing::Size(100, 20); this->txtSy->Text = L"1";
            this->txtAngulo->Location = System::Drawing::Point(150, 327); this->txtAngulo->Size = System::Drawing::Size(100, 20); this->txtAngulo->Text = L"0";
            this->txtShx->Location = System::Drawing::Point(150, 357); this->txtShx->Size = System::Drawing::Size(100, 20); this->txtShx->Text = L"0";
            this->txtShy->Location = System::Drawing::Point(150, 387); this->txtShy->Size = System::Drawing::Size(100, 20); this->txtShy->Text = L"0";

            this->lblTx->AutoSize = true; this->lblTx->Location = System::Drawing::Point(20, 210); this->lblTx->Text = L"Traslación X (Tx):";
            this->lblTy->AutoSize = true; this->lblTy->Location = System::Drawing::Point(20, 240); this->lblTy->Text = L"Traslación Y (Ty):";
            this->lblSx->AutoSize = true; this->lblSx->Location = System::Drawing::Point(20, 270); this->lblSx->Text = L"Escala X (Sx):";
            this->lblSy->AutoSize = true; this->lblSy->Location = System::Drawing::Point(20, 300); this->lblSy->Text = L"Escala Y (Sy):";
            this->lblAngulo->AutoSize = true; this->lblAngulo->Location = System::Drawing::Point(20, 330); this->lblAngulo->Text = L"Ángulo (°):";
            this->lblShx->AutoSize = true; this->lblShx->Location = System::Drawing::Point(20, 360); this->lblShx->Text = L"Cizallamiento X:";
            this->lblShy->AutoSize = true; this->lblShy->Location = System::Drawing::Point(20, 390); this->lblShy->Text = L"Cizallamiento Y:";

            // Form
            this->ClientSize = System::Drawing::Size(864, 681);
            this->Controls->Add(this->canvasBox);
            this->Controls->Add(this->cmbObjetos);
            this->Controls->Add(this->btnCargar);
            this->Controls->Add(this->btnSpawnAleatorio);
            this->Controls->Add(this->lblCoordX);
            this->Controls->Add(this->txtCoordX);
            this->Controls->Add(this->lblCoordY);
            this->Controls->Add(this->txtCoordY);
            this->Controls->Add(this->btnAgregarPunto);
            this->Controls->Add(this->lblContadorPuntos);
            this->Controls->Add(this->btnLimpiarPuntos);
            this->Controls->Add(this->btnPoligonoAleatorio);
            this->Controls->Add(this->lblTx);
            this->Controls->Add(this->txtTx);
            this->Controls->Add(this->lblTy);
            this->Controls->Add(this->txtTy);
            this->Controls->Add(this->lblSx);
            this->Controls->Add(this->txtSx);
            this->Controls->Add(this->lblSy);
            this->Controls->Add(this->txtSy);
            this->Controls->Add(this->lblAngulo);
            this->Controls->Add(this->txtAngulo);
            this->Controls->Add(this->lblShx);
            this->Controls->Add(this->txtShx);
            this->Controls->Add(this->lblShy);
            this->Controls->Add(this->txtShy);
            this->Controls->Add(this->chkReflejoX);
            this->Controls->Add(this->chkReflejoY);
            this->Controls->Add(this->btnTransformar);
            this->Controls->Add(this->btnReiniciarTransformaciones);
            this->Controls->Add(this->lblTituloPasos);
            this->Controls->Add(this->lstPasos);
            this->Controls->Add(this->btnCerrar);
            this->Name = L"MyForm";
            this->Text = L"Programa para graficar las transformaciones lineales";
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        Punto2D AplicarMatriz(double M[3][3], Punto2D p) {
            Punto2D res;
            res.x = M[0][0] * p.x + M[0][1] * p.y + M[0][2] * 1.0;
            res.y = M[1][0] * p.x + M[1][1] * p.y + M[1][2] * 1.0;
            return res;
        }

        std::vector<Punto2D> AplicarMatrizAVectores(double M[3][3], const std::vector<Punto2D>& entrada) {
            std::vector<Punto2D> salida;
            for (const auto& p : entrada) {
                salida.push_back(AplicarMatriz(M, p));
            }
            return salida;
        }

    private:
        System::Void canvasBox_Resize(System::Object^ sender, System::EventArgs^ e) {
            canvasBox->Invalidate();
        }

        System::Void canvasBox_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->Clear(Color::White);

            // 1. DIBUJAR IMAGEN DE FONDO CON TRANSPARENCIA
            if (imagenFondo != nullptr) {
                // Nivel de opacidad: 0.3f = 30% visible (sutil), 0.5f = 50% transparente
                float opacidad = 0.35f;

                // Definir la matriz de color modificando el canal Alpha (matriz 5x5)
                cli::array<cli::array<float>^>^ elementosMatriz = {
                    gcnew cli::array<float>{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
                    gcnew cli::array<float>{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
                    gcnew cli::array<float>{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
                    gcnew cli::array<float>{ 0.0f, 0.0f, 0.0f, opacidad, 0.0f }, // Alpha / Transparencia
                    gcnew cli::array<float>{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
                };

                ColorMatrix^ matrizColor = gcnew ColorMatrix(elementosMatriz);
                ImageAttributes^ atributosImagen = gcnew ImageAttributes();
                atributosImagen->SetColorMatrix(matrizColor, ColorMatrixFlag::Default, ColorAdjustType::Bitmap);

                // Dibujar la imagen aplicando la transparencia
                System::Drawing::Rectangle destRect(0, 0, canvasBox->Width, canvasBox->Height);
                g->DrawImage(
                    imagenFondo,
                    destRect,
                    0, 0, imagenFondo->Width, imagenFondo->Height,
                    GraphicsUnit::Pixel,
                    atributosImagen
                );

                // Liberar recursos temporales de GDI+
                delete atributosImagen;
                delete matrizColor;
            }

            int centroX = canvasBox->Width / 2;
            int centroY = canvasBox->Height / 2;

            // 2. DIBUJAR EJES CARTESIANOS
            Pen^ penEjes = gcnew Pen(Color::LightGray, 1.0f);
            g->DrawLine(penEjes, 0, centroY, canvasBox->Width, centroY);
            g->DrawLine(penEjes, centroX, 0, centroX, canvasBox->Height);

            // 3. DIBUJAR VÉRTICES TRANSFORMADOS
            if (verticesTransformados->empty()) return;

            Pen^ penFigura = gcnew Pen(Color::Blue, 2.0f);
            for (size_t i = 0; i < verticesTransformados->size() - 1; ++i) {
                int x1 = centroX + (int)std::round((*verticesTransformados)[i].x);
                int y1 = centroY - (int)std::round((*verticesTransformados)[i].y);
                int x2 = centroX + (int)std::round((*verticesTransformados)[i + 1].x);
                int y2 = centroY - (int)std::round((*verticesTransformados)[i + 1].y);

                g->DrawLine(penFigura, x1, y1, x2, y2);
            }
        }

        System::Void CargarFiguraBase() {
            int opcion = cmbObjetos->SelectedIndex;

            if (opcion != 4) {
                verticesOriginales->clear();
            }

            switch (opcion) {
            case 0: // Línea
                *verticesOriginales = { {0, 0}, {100, 100} };
                break;
            case 1: // Casa
                *verticesOriginales = { {0,0}, {60,0}, {60,50}, {30,80}, {0,50}, {0,0} };
                break;
            case 2: // Auto
                *verticesOriginales = { {-50,0}, {50,0}, {50,15}, {30,15}, {20,35}, {-20,35}, {-30,15}, {-50,15}, {-50,0} };
                break;
            case 3: // Perro
                *verticesOriginales = {
                    {-120, 0}, {-120, 60}, {40, 60}, {40, 150}, {55, 150},
                    {55, 130}, {80, 130}, {80, 110}, {120, 110}, {120, 75},
                    {80, 60}, {80, 0}, {55, 0}, {55, 30}, {30, 10},
                    {0, 10}, {-90, 30}, {-90, 0}, {-120, 0}
                };
                break;
            case 4: // Personalizado
                break;
            }

            contadorPuntos = (int)verticesOriginales->size();
            lblContadorPuntos->Text = L"Puntos agregados: " + contadorPuntos.ToString();

            *verticesTransformados = *verticesOriginales;
            historialPasos->clear();
            lstPasos->Items->Clear();
        }

        System::Void btnCargar_Click(System::Object^ sender, System::EventArgs^ e) {
            CargarFiguraBase();
            canvasBox->Invalidate();
        }

        System::Void btnSpawnAleatorio_Click(System::Object^ sender, System::EventArgs^ e) {
            if (verticesOriginales->empty()) {
                CargarFiguraBase();
            }
            if (verticesOriginales->empty()) return;

            double randTx = (rand() % 301) - 150;
            double randTy = (rand() % 301) - 150;

            txtTx->Text = randTx.ToString();
            txtTy->Text = randTy.ToString();

            btnTransformar_Click(sender, e);
        }

        System::Void btnAgregarPunto_Click(System::Object^ sender, System::EventArgs^ e) {
            try {
                double x = Convert::ToDouble(txtCoordX->Text);
                double y = Convert::ToDouble(txtCoordY->Text);

                verticesOriginales->push_back({ x, y });
                contadorPuntos = (int)verticesOriginales->size();

                lblContadorPuntos->Text = L"Puntos agregados: " + contadorPuntos.ToString();
                cmbObjetos->SelectedIndex = 4;

                *verticesTransformados = *verticesOriginales;
                canvasBox->Invalidate();
            }
            catch (...) {
                MessageBox::Show(L"Ingrese números válidos para las coordenadas.");
            }
        }

        System::Void btnLimpiarPuntos_Click(System::Object^ sender, System::EventArgs^ e) {
            verticesOriginales->clear();
            verticesTransformados->clear();
            historialPasos->clear();
            lstPasos->Items->Clear();
            contadorPuntos = 0;
            lblContadorPuntos->Text = L"Puntos agregados: 0";
            canvasBox->Invalidate();
        }

        System::Void btnPoligonoAleatorio_Click(System::Object^ sender, System::EventArgs^ e) {
            verticesOriginales->clear();
            int numPuntos = 5 + (rand() % 4);

            for (int i = 0; i < numPuntos; ++i) {
                double x = (rand() % 200) - 100;
                double y = (rand() % 200) - 100;
                verticesOriginales->push_back({ x, y });
            }
            if (!verticesOriginales->empty()) {
                verticesOriginales->push_back((*verticesOriginales)[0]);
            }

            contadorPuntos = (int)verticesOriginales->size();
            lblContadorPuntos->Text = L"Puntos agregados: " + contadorPuntos.ToString();
            cmbObjetos->SelectedIndex = 4;

            *verticesTransformados = *verticesOriginales;
            historialPasos->clear();
            lstPasos->Items->Clear();
            canvasBox->Invalidate();
        }

        System::Void btnReiniciarTransformaciones_Click(System::Object^ sender, System::EventArgs^ e) {
            txtTx->Text = L"0";
            txtTy->Text = L"0";
            txtSx->Text = L"1";
            txtSy->Text = L"1";
            txtAngulo->Text = L"0";
            txtShx->Text = L"0";
            txtShy->Text = L"0";
            chkReflejoX->Checked = false;
            chkReflejoY->Checked = false;

            historialPasos->clear();
            lstPasos->Items->Clear();

            *verticesTransformados = *verticesOriginales;
            canvasBox->Invalidate();
        }

        System::Void btnTransformar_Click(System::Object^ sender, System::EventArgs^ e) {
            try {
                double tx = Convert::ToDouble(txtTx->Text);
                double ty = Convert::ToDouble(txtTy->Text);
                double sx = Convert::ToDouble(txtSx->Text);
                double sy = Convert::ToDouble(txtSy->Text);
                double anguloGrad = Convert::ToDouble(txtAngulo->Text);
                double rad = anguloGrad * 3.14159265358979323846 / 180.0;
                double shx = Convert::ToDouble(txtShx->Text);
                double shy = Convert::ToDouble(txtShy->Text);

                lstPasos->Items->Clear();
                historialPasos->clear();

                std::vector<Punto2D> estadoActual = *verticesOriginales;

                // Paso 0
                String^ str0 = L"0. Estado Inicial (Sin transformaciones)";
                PasoTransformacion paso0;
                paso0.descripcion = SystemToStdString(str0);
                paso0.verticesEstado = estadoActual;
                historialPasos->push_back(paso0);
                lstPasos->Items->Add(str0);

                int numPaso = 1;

                // 1. Escala
                if (sx != 1.0 || sy != 1.0) {
                    double S[3][3] = { {sx, 0, 0}, {0, sy, 0}, {0, 0, 1} };
                    estadoActual = AplicarMatrizAVectores(S, estadoActual);

                    String^ strDesc = numPaso.ToString() + L". Escala (Sx=" + sx.ToString() + L", Sy=" + sy.ToString() + L") -> Afecta al tamaño local";
                    PasoTransformacion paso;
                    paso.descripcion = SystemToStdString(strDesc);
                    paso.verticesEstado = estadoActual;
                    historialPasos->push_back(paso);
                    lstPasos->Items->Add(strDesc);
                    numPaso++;
                }

                // 2. Reflexión
                if (chkReflejoX->Checked || chkReflejoY->Checked) {
                    double rfX = chkReflejoY->Checked ? -1.0 : 1.0;
                    double rfY = chkReflejoX->Checked ? -1.0 : 1.0;

                    double Ref[3][3] = { {rfX, 0, 0}, {0, rfY, 0}, {0, 0, 1} };
                    estadoActual = AplicarMatrizAVectores(Ref, estadoActual);

                    String^ ejes = L"";
                    if (chkReflejoX->Checked && chkReflejoY->Checked) ejes = L"Eje X y Eje Y";
                    else if (chkReflejoX->Checked) ejes = L"Eje X";
                    else ejes = L"Eje Y";

                    String^ strDesc = numPaso.ToString() + L". Reflexión (" + ejes + L") -> Invierte la orientación";
                    PasoTransformacion paso;
                    paso.descripcion = SystemToStdString(strDesc);
                    paso.verticesEstado = estadoActual;
                    historialPasos->push_back(paso);
                    lstPasos->Items->Add(strDesc);
                    numPaso++;
                }

                // 3. Cizallamiento
                if (shx != 0.0 || shy != 0.0) {
                    double Sh[3][3] = { {1, shx, 0}, {shy, 1, 0}, {0, 0, 1} };
                    estadoActual = AplicarMatrizAVectores(Sh, estadoActual);

                    String^ strDesc = numPaso.ToString() + L". Cizallamiento (Shx=" + shx.ToString() + L", Shy=" + shy.ToString() + L") -> Deforma ejes";
                    PasoTransformacion paso;
                    paso.descripcion = SystemToStdString(strDesc);
                    paso.verticesEstado = estadoActual;
                    historialPasos->push_back(paso);
                    lstPasos->Items->Add(strDesc);
                    numPaso++;
                }

                // 4. Rotación
                if (anguloGrad != 0.0) {
                    double R[3][3] = { {cos(rad), -sin(rad), 0}, {sin(rad), cos(rad), 0}, {0, 0, 1} };
                    estadoActual = AplicarMatrizAVectores(R, estadoActual);

                    String^ strDesc = numPaso.ToString() + L". Rotación (" + anguloGrad.ToString() + L"°) -> Gira en el origen";
                    PasoTransformacion paso;
                    paso.descripcion = SystemToStdString(strDesc);
                    paso.verticesEstado = estadoActual;
                    historialPasos->push_back(paso);
                    lstPasos->Items->Add(strDesc);
                    numPaso++;
                }

                // 5. Traslación
                if (tx != 0.0 || ty != 0.0) {
                    double T[3][3] = { {1, 0, tx}, {0, 1, ty}, {0, 0, 1} };
                    estadoActual = AplicarMatrizAVectores(T, estadoActual);

                    String^ strDesc = numPaso.ToString() + L". Traslación (Tx=" + tx.ToString() + L", Ty=" + ty.ToString() + L") -> Mueve la posición final";
                    PasoTransformacion paso;
                    paso.descripcion = SystemToStdString(strDesc);
                    paso.verticesEstado = estadoActual;
                    historialPasos->push_back(paso);
                    lstPasos->Items->Add(strDesc);
                    numPaso++;
                }

                *verticesTransformados = estadoActual;
                if (lstPasos->Items->Count > 0) {
                    lstPasos->SelectedIndex = lstPasos->Items->Count - 1;
                }

                canvasBox->Invalidate();
            }
            catch (...) {
                MessageBox::Show(L"Revise los campos numéricos de las transformaciones.");
            }
        }

        System::Void lstPasos_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
            int index = lstPasos->SelectedIndex;
            if (index >= 0 && index < (int)historialPasos->size()) {
                *verticesTransformados = (*historialPasos)[index].verticesEstado;
                canvasBox->Invalidate();
            }
        }

        System::Void btnCerrar_Click(System::Object^ sender, System::EventArgs^ e) {
            this->Close();
        }
    };
}