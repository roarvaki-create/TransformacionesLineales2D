#pragma once

#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

#include "Punto2D.h"
#include "DoubleBufferedPanel.h"
#include "FormCreadorAnimacion.h"

namespace TransformacionesLineales2D {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Drawing::Imaging;

    enum TipoTransformacion {
        NINGUNA,
        ESCALA,
        REFLEXION,
        CIZALLAMIENTO,
        ROTACION,
        TRASLACION
    };

    struct PasoTransformacion {
        TipoTransformacion tipo;
        std::string descripcion;
        std::vector<Punto2D> verticesEstado;
    };

    public ref class MyForm : public System::Windows::Forms::Form
    {
    private:
        std::vector<Punto2D>* verticesOriginales;
        std::vector<Punto2D>* verticesTransformados;
        std::vector<PasoTransformacion>* historialPasos;
        std::vector<ConfiguracionFrame>* secuenciaAnimacionSecundaria;
        int contadorPuntos;

        System::Drawing::Image^ imagenFondo;

        // Control de Animación
        System::Windows::Forms::Timer^ timerAnimacion;
        int pasoAnimacionActual;
        float progresoPaso;
        bool modoAnimacionPorFrames;

        // Componentes GUI
        System::Windows::Forms::Button^ btnAbrirCreadorFrames;
        System::Windows::Forms::ComboBox^ cmbObjetos;
        System::Windows::Forms::Button^ btnCargar;
        System::Windows::Forms::Button^ btnSpawnAleatorio;
        System::Windows::Forms::Button^ btnTransformar;
        System::Windows::Forms::Button^ btnReiniciarTransformaciones;
        System::Windows::Forms::Button^ btnPoligonoAleatorio;
        System::Windows::Forms::Button^ btnAnimarPasos;
        System::Windows::Forms::Button^ btnCerrar;
        DoubleBufferedPanel^ canvasBox;

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

            String^ nombreImagenFondo = L"wha.png";

            try {
                imagenFondo = Image::FromFile(nombreImagenFondo);
            }
            catch (...) {
                imagenFondo = nullptr;
            }

            verticesOriginales = new std::vector<Punto2D>();
            verticesTransformados = new std::vector<Punto2D>();
            historialPasos = new std::vector<PasoTransformacion>();
            secuenciaAnimacionSecundaria = new std::vector<ConfiguracionFrame>();
            contadorPuntos = 0;

            timerAnimacion = gcnew System::Windows::Forms::Timer();
            timerAnimacion->Interval = 30; // Animación fluida
            timerAnimacion->Tick += gcnew System::EventHandler(this, &MyForm::timerAnimacion_Tick);
            pasoAnimacionActual = 0;
            progresoPaso = 0.0f;
            modoAnimacionPorFrames = false;

            cmbObjetos->SelectedIndex = 1;
            CargarFiguraBase();
        }

    protected:
        ~MyForm()
        {
            if (components) delete components;

            if (timerAnimacion != nullptr) {
                timerAnimacion->Stop();
                delete timerAnimacion;
                timerAnimacion = nullptr;
            }

            if (imagenFondo != nullptr) {
                delete imagenFondo;
                imagenFondo = nullptr;
            }

            if (verticesOriginales != nullptr) {
                verticesOriginales->clear();
                verticesOriginales->shrink_to_fit();
                delete verticesOriginales;
                verticesOriginales = nullptr;
            }
            if (verticesTransformados != nullptr) {
                verticesTransformados->clear();
                verticesTransformados->shrink_to_fit();
                delete verticesTransformados;
                verticesTransformados = nullptr;
            }
            if (historialPasos != nullptr) {
                historialPasos->clear();
                historialPasos->shrink_to_fit();
                delete historialPasos;
                historialPasos = nullptr;
            }
            if (secuenciaAnimacionSecundaria != nullptr) {
                for (auto& frame : *secuenciaAnimacionSecundaria) {
                    frame.verticesResultado.clear();
                    frame.verticesResultado.shrink_to_fit();
                }
                secuenciaAnimacionSecundaria->clear();
                secuenciaAnimacionSecundaria->shrink_to_fit();
                delete secuenciaAnimacionSecundaria;
                secuenciaAnimacionSecundaria = nullptr;
            }
        }

    private:
        System::ComponentModel::Container^ components;

        void EstilarBoton(System::Windows::Forms::Button^ btn, Color backColor, Color textColor) {
            btn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            btn->FlatAppearance->BorderSize = 0;
            btn->BackColor = backColor;
            btn->ForeColor = textColor;
            btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8, System::Drawing::FontStyle::Bold));
        }

        void EstilarTextBox(System::Windows::Forms::TextBox^ txt) {
            txt->BackColor = Color::FromArgb(45, 45, 48);
            txt->ForeColor = Color::FromArgb(0, 240, 255);
            txt->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            txt->Font = (gcnew System::Drawing::Font(L"Consolas", 8, System::Drawing::FontStyle::Bold));
        }

        void EstilarLabel(System::Windows::Forms::Label^ lbl) {
            lbl->ForeColor = Color::FromArgb(200, 200, 200);
            lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8, System::Drawing::FontStyle::Regular));
        }

        void InitializeComponent(void)
        {
            this->btnAbrirCreadorFrames = (gcnew System::Windows::Forms::Button());
            this->cmbObjetos = (gcnew System::Windows::Forms::ComboBox());
            this->btnCargar = (gcnew System::Windows::Forms::Button());
            this->btnSpawnAleatorio = (gcnew System::Windows::Forms::Button());
            this->btnPoligonoAleatorio = (gcnew System::Windows::Forms::Button());
            this->btnTransformar = (gcnew System::Windows::Forms::Button());
            this->btnReiniciarTransformaciones = (gcnew System::Windows::Forms::Button());
            this->btnAnimarPasos = (gcnew System::Windows::Forms::Button());
            this->btnCerrar = (gcnew System::Windows::Forms::Button());
            this->canvasBox = (gcnew DoubleBufferedPanel());
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

            this->BackColor = Color::FromArgb(18, 18, 18);

            this->btnAbrirCreadorFrames->Location = System::Drawing::Point(15, 12);
            this->btnAbrirCreadorFrames->Name = L"btnAbrirCreadorFrames";
            this->btnAbrirCreadorFrames->Size = System::Drawing::Size(240, 30);
            this->btnAbrirCreadorFrames->Text = L"🛠 Creador de Animación por Frames";
            EstilarBoton(this->btnAbrirCreadorFrames, Color::FromArgb(180, 100, 255), Color::White);
            this->btnAbrirCreadorFrames->Click += gcnew System::EventHandler(this, &MyForm::btnAbrirCreadorFrames_Click);

            this->cmbObjetos->FormattingEnabled = true;
            this->cmbObjetos->Items->AddRange(gcnew cli::array< System::Object^ >(5) { L"Línea (0,0)", L"Casa", L"Auto", L"Perro", L"Personalizado" });
            this->cmbObjetos->Location = System::Drawing::Point(15, 48);
            this->cmbObjetos->Name = L"cmbObjetos";
            this->cmbObjetos->Size = System::Drawing::Size(240, 21);
            this->cmbObjetos->BackColor = Color::FromArgb(45, 45, 48);
            this->cmbObjetos->ForeColor = Color::White;

            this->btnCargar->Location = System::Drawing::Point(15, 75);
            this->btnCargar->Size = System::Drawing::Size(115, 25);
            this->btnCargar->Text = L"Cargar Objeto";
            EstilarBoton(this->btnCargar, Color::FromArgb(35, 35, 40), Color::FromArgb(0, 240, 255));
            this->btnCargar->Click += gcnew System::EventHandler(this, &MyForm::btnCargar_Click);

            this->btnSpawnAleatorio->Location = System::Drawing::Point(140, 75);
            this->btnSpawnAleatorio->Size = System::Drawing::Size(115, 25);
            this->btnSpawnAleatorio->Text = L"Spawn Aleatorio";
            EstilarBoton(this->btnSpawnAleatorio, Color::FromArgb(35, 35, 40), Color::FromArgb(180, 100, 255));
            this->btnSpawnAleatorio->Click += gcnew System::EventHandler(this, &MyForm::btnSpawnAleatorio_Click);

            this->lblCoordX->AutoSize = true; this->lblCoordX->Location = System::Drawing::Point(15, 108); this->lblCoordX->Text = L"X:"; EstilarLabel(this->lblCoordX);
            this->txtCoordX->Location = System::Drawing::Point(32, 105); this->txtCoordX->Size = System::Drawing::Size(35, 20); this->txtCoordX->Text = L"0"; EstilarTextBox(this->txtCoordX);

            this->lblCoordY->AutoSize = true; this->lblCoordY->Location = System::Drawing::Point(72, 108); this->lblCoordY->Text = L"Y:"; EstilarLabel(this->lblCoordY);
            this->txtCoordY->Location = System::Drawing::Point(89, 105); this->txtCoordY->Size = System::Drawing::Size(35, 20); this->txtCoordY->Text = L"0"; EstilarTextBox(this->txtCoordY);

            this->btnAgregarPunto->Location = System::Drawing::Point(130, 103); this->btnAgregarPunto->Size = System::Drawing::Size(125, 24); this->btnAgregarPunto->Text = L"Agregar Punto";
            EstilarBoton(this->btnAgregarPunto, Color::FromArgb(45, 45, 50), Color::FromArgb(0, 240, 255));
            this->btnAgregarPunto->Click += gcnew System::EventHandler(this, &MyForm::btnAgregarPunto_Click);

            this->lblContadorPuntos->AutoSize = true; this->lblContadorPuntos->Location = System::Drawing::Point(15, 130); this->lblContadorPuntos->Text = L"Puntos agregados: 0"; EstilarLabel(this->lblContadorPuntos);

            this->btnLimpiarPuntos->Location = System::Drawing::Point(15, 148); this->btnLimpiarPuntos->Size = System::Drawing::Size(240, 24); this->btnLimpiarPuntos->Text = L"Limpiar Coordenadas Manuales";
            EstilarBoton(this->btnLimpiarPuntos, Color::FromArgb(45, 45, 50), Color::FromArgb(255, 100, 100));
            this->btnLimpiarPuntos->Click += gcnew System::EventHandler(this, &MyForm::btnLimpiarPuntos_Click);

            this->btnPoligonoAleatorio->Location = System::Drawing::Point(15, 176);
            this->btnPoligonoAleatorio->Size = System::Drawing::Size(240, 24);
            this->btnPoligonoAleatorio->Text = L"Generar Puntos Aleatorios";
            EstilarBoton(this->btnPoligonoAleatorio, Color::FromArgb(35, 35, 40), Color::FromArgb(255, 200, 0));
            this->btnPoligonoAleatorio->Click += gcnew System::EventHandler(this, &MyForm::btnPoligonoAleatorio_Click);

            int yPos = 208;
            this->lblTx->AutoSize = true; this->lblTx->Location = System::Drawing::Point(15, yPos + 3); this->lblTx->Text = L"Traslación X (Tx):"; EstilarLabel(this->lblTx);
            this->txtTx->Location = System::Drawing::Point(155, yPos); this->txtTx->Size = System::Drawing::Size(100, 20); this->txtTx->Text = L"0"; EstilarTextBox(this->txtTx); yPos += 26;

            this->lblTy->AutoSize = true; this->lblTy->Location = System::Drawing::Point(15, yPos + 3); this->lblTy->Text = L"Traslación Y (Ty):"; EstilarLabel(this->lblTy);
            this->txtTy->Location = System::Drawing::Point(155, yPos); this->txtTy->Size = System::Drawing::Size(100, 20); this->txtTy->Text = L"0"; EstilarTextBox(this->txtTy); yPos += 26;

            this->lblSx->AutoSize = true; this->lblSx->Location = System::Drawing::Point(15, yPos + 3); this->lblSx->Text = L"Escala X (Sx):"; EstilarLabel(this->lblSx);
            this->txtSx->Location = System::Drawing::Point(155, yPos); this->txtSx->Size = System::Drawing::Size(100, 20); this->txtSx->Text = L"1"; EstilarTextBox(this->txtSx); yPos += 26;

            this->lblSy->AutoSize = true; this->lblSy->Location = System::Drawing::Point(15, yPos + 3); this->lblSy->Text = L"Escala Y (Sy):"; EstilarLabel(this->lblSy);
            this->txtSy->Location = System::Drawing::Point(155, yPos); this->txtSy->Size = System::Drawing::Size(100, 20); this->txtSy->Text = L"1"; EstilarTextBox(this->txtSy); yPos += 26;

            this->lblAngulo->AutoSize = true; this->lblAngulo->Location = System::Drawing::Point(15, yPos + 3); this->lblAngulo->Text = L"Ángulo (°):"; EstilarLabel(this->lblAngulo);
            this->txtAngulo->Location = System::Drawing::Point(155, yPos); this->txtAngulo->Size = System::Drawing::Size(100, 20); this->txtAngulo->Text = L"0"; EstilarTextBox(this->txtAngulo); yPos += 26;

            this->lblShx->AutoSize = true; this->lblShx->Location = System::Drawing::Point(15, yPos + 3); this->lblShx->Text = L"Cizallamiento X:"; EstilarLabel(this->lblShx);
            this->txtShx->Location = System::Drawing::Point(155, yPos); this->txtShx->Size = System::Drawing::Size(100, 20); this->txtShx->Text = L"0"; EstilarTextBox(this->txtShx); yPos += 26;

            this->lblShy->AutoSize = true; this->lblShy->Location = System::Drawing::Point(15, yPos + 3); this->lblShy->Text = L"Cizallamiento Y:"; EstilarLabel(this->lblShy);
            this->txtShy->Location = System::Drawing::Point(155, yPos); this->txtShy->Size = System::Drawing::Size(100, 20); this->txtShy->Text = L"0"; EstilarTextBox(this->txtShy); yPos += 26;

            this->chkReflejoX->AutoSize = true; this->chkReflejoX->Location = System::Drawing::Point(15, yPos); this->chkReflejoX->Text = L"Reflejo en Eje X"; this->chkReflejoX->ForeColor = Color::FromArgb(200, 200, 200);
            this->chkReflejoY->AutoSize = true; this->chkReflejoY->Location = System::Drawing::Point(135, yPos); this->chkReflejoY->Text = L"Reflejo en Eje Y"; this->chkReflejoY->ForeColor = Color::FromArgb(200, 200, 200); yPos += 24;

            this->btnTransformar->Location = System::Drawing::Point(15, yPos);
            this->btnTransformar->Size = System::Drawing::Size(115, 28);
            this->btnTransformar->Text = L"Transformar";
            EstilarBoton(this->btnTransformar, Color::FromArgb(0, 150, 200), Color::White);
            this->btnTransformar->Click += gcnew System::EventHandler(this, &MyForm::btnTransformar_Click);

            this->btnReiniciarTransformaciones->Location = System::Drawing::Point(140, yPos);
            this->btnReiniciarTransformaciones->Size = System::Drawing::Size(115, 28);
            this->btnReiniciarTransformaciones->Text = L"Reiniciar Valores";
            EstilarBoton(this->btnReiniciarTransformaciones, Color::FromArgb(60, 60, 65), Color::White);
            this->btnReiniciarTransformaciones->Click += gcnew System::EventHandler(this, &MyForm::btnReiniciarTransformaciones_Click); yPos += 32;

            this->btnAnimarPasos->Location = System::Drawing::Point(15, yPos);
            this->btnAnimarPasos->Size = System::Drawing::Size(240, 26);
            this->btnAnimarPasos->Text = L"Animar Transformación";
            EstilarBoton(this->btnAnimarPasos, Color::FromArgb(0, 200, 100), Color::Black);
            this->btnAnimarPasos->Click += gcnew System::EventHandler(this, &MyForm::btnAnimarPasos_Click); yPos += 30;

            this->lblTituloPasos->AutoSize = true;
            this->lblTituloPasos->Location = System::Drawing::Point(15, yPos);
            this->lblTituloPasos->Text = L"Pasos del proceso (Haz click para ver):";
            EstilarLabel(this->lblTituloPasos); yPos += 18;

            this->lstPasos->FormattingEnabled = true;
            this->lstPasos->HorizontalScrollbar = true;
            this->lstPasos->Location = System::Drawing::Point(15, yPos);
            this->lstPasos->Size = System::Drawing::Size(240, 56);
            this->lstPasos->BackColor = Color::FromArgb(30, 30, 35);
            this->lstPasos->ForeColor = Color::FromArgb(0, 240, 255);
            this->lstPasos->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->lstPasos->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::lstPasos_SelectedIndexChanged); yPos += 62;

            this->btnCerrar->Location = System::Drawing::Point(15, yPos);
            this->btnCerrar->Size = System::Drawing::Size(240, 32);
            this->btnCerrar->Text = L"Cerrar Aplicación";
            EstilarBoton(this->btnCerrar, Color::FromArgb(200, 40, 60), Color::White);
            this->btnCerrar->Click += gcnew System::EventHandler(this, &MyForm::btnCerrar_Click);

            this->canvasBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
                | System::Windows::Forms::AnchorStyles::Left)
                | System::Windows::Forms::AnchorStyles::Right));
            this->canvasBox->BackColor = Color::FromArgb(5, 5, 5);
            this->canvasBox->Location = System::Drawing::Point(270, 12);
            this->canvasBox->Size = System::Drawing::Size(575, 635);
            this->canvasBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::canvasBox_Paint);
            this->canvasBox->Resize += gcnew System::EventHandler(this, &MyForm::canvasBox_Resize);

            this->ClientSize = System::Drawing::Size(860, 660);
            this->Controls->Add(this->btnAbrirCreadorFrames);
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
            this->Controls->Add(this->btnAnimarPasos);
            this->Controls->Add(this->lblTituloPasos);
            this->Controls->Add(this->lstPasos);
            this->Controls->Add(this->btnCerrar);
            this->Name = L"MyForm";
            this->Text = L"Programa para graficar las transformaciones lineales";
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        System::Void timerAnimacion_Tick(System::Object^ sender, System::EventArgs^ e) {
            if (modoAnimacionPorFrames) {
                // MODULO FRAMES
                if (secuenciaAnimacionSecundaria == nullptr || secuenciaAnimacionSecundaria->empty()) {
                    timerAnimacion->Stop();
                    return;
                }

                progresoPaso += 0.05f;

                if (progresoPaso >= 1.0f) {
                    progresoPaso = 0.0f;
                    pasoAnimacionActual++;
                    if (pasoAnimacionActual >= (int)secuenciaAnimacionSecundaria->size() - 1) {
                        pasoAnimacionActual = (int)secuenciaAnimacionSecundaria->size() - 1;
                        timerAnimacion->Stop();
                    }
                    if (pasoAnimacionActual < (int)secuenciaAnimacionSecundaria->size()) {
                        lstPasos->SelectedIndex = pasoAnimacionActual;
                        *verticesTransformados = (*secuenciaAnimacionSecundaria)[pasoAnimacionActual].verticesResultado;
                    }
                    canvasBox->Invalidate();
                    return;
                }

                int idxA = pasoAnimacionActual;
                int idxB = pasoAnimacionActual + 1;

                if (idxB >= (int)secuenciaAnimacionSecundaria->size()) {
                    timerAnimacion->Stop();
                    return;
                }

                const ConfiguracionFrame& fA = (*secuenciaAnimacionSecundaria)[idxA];
                const ConfiguracionFrame& fB = (*secuenciaAnimacionSecundaria)[idxB];

                if (!fB.animarTransicion) {
                    *verticesTransformados = fB.verticesResultado;
                    canvasBox->Invalidate();
                    return;
                }

                double origX = fA.origenX + progresoPaso * (fB.origenX - fA.origenX);
                double origY = fA.origenY + progresoPaso * (fB.origenY - fA.origenY);
                double tx = fA.tx + progresoPaso * (fB.tx - fA.tx);
                double ty = fA.ty + progresoPaso * (fB.ty - fA.ty);
                double sx = fA.sx + progresoPaso * (fB.sx - fA.sx);
                double sy = fA.sy + progresoPaso * (fB.sy - fA.sy);
                double ang = fA.angulo + progresoPaso * (fB.angulo - fA.angulo);
                double shx = fA.shx + progresoPaso * (fB.shx - fA.shx);
                double shy = fA.shy + progresoPaso * (fB.shy - fA.shy);

                const std::vector<Punto2D>& basePts = (fB.acumularDesdeAnterior) ? fA.verticesResultado : *verticesOriginales;

                std::vector<Punto2D> temp;
                double rad = ang * 3.14159265358979323846 / 180.0;
                double S[2][2] = { {sx, 0.0}, {0.0, sy} };
                double Ref[2][2] = { {fB.reflejoY ? -1.0 : 1.0, 0.0}, {0.0, fB.reflejoX ? -1.0 : 1.0} };
                double Sh[2][2] = { {1.0, shx}, {shy, 1.0} };
                double R[2][2] = { {cos(rad), -sin(rad)}, {sin(rad), cos(rad)} };

                for (size_t i = 0; i < basePts.size(); ++i) {
                    Punto2D pt = basePts[i];
                    double px = pt.x + origX;
                    double py = pt.y + origY;

                    Punto2D p1 = { S[0][0] * px + S[0][1] * py, S[1][0] * px + S[1][1] * py };
                    Punto2D p2 = { Ref[0][0] * p1.x + Ref[0][1] * p1.y, Ref[1][0] * p1.x + Ref[1][1] * p1.y };
                    Punto2D p3 = { Sh[0][0] * p2.x + Sh[0][1] * p2.y, Sh[1][0] * p2.x + Sh[1][1] * p2.y };
                    Punto2D p4 = { R[0][0] * p3.x + R[0][1] * p3.y, R[1][0] * p3.x + R[1][1] * p3.y };

                    temp.push_back({ p4.x + tx, p4.y + ty });
                }

                *verticesTransformados = temp;
                canvasBox->Invalidate();
            }
            else {
                // ANIMACIÓN POR PASOS SECUENCIALES (VENTANA PRINCIPAL)
                if (historialPasos == nullptr || historialPasos->empty()) {
                    timerAnimacion->Stop();
                    return;
                }

                progresoPaso += 0.06f;

                if (progresoPaso >= 1.0f) {
                    progresoPaso = 0.0f;
                    pasoAnimacionActual++;

                    if (pasoAnimacionActual >= (int)historialPasos->size()) {
                        pasoAnimacionActual = (int)historialPasos->size() - 1;
                        timerAnimacion->Stop();
                    }

                    lstPasos->SelectedIndex = pasoAnimacionActual;
                    *verticesTransformados = (*historialPasos)[pasoAnimacionActual].verticesEstado;
                    canvasBox->Invalidate();
                    return;
                }

                int idxA = pasoAnimacionActual - 1;
                int idxB = pasoAnimacionActual;

                std::vector<Punto2D> pA = (idxA < 0) ? *verticesOriginales : (*historialPasos)[idxA].verticesEstado;
                std::vector<Punto2D> pB = (*historialPasos)[idxB].verticesEstado;

                std::vector<Punto2D> temp;
                for (size_t i = 0; i < pA.size(); ++i) {
                    double x = pA[i].x + progresoPaso * (pB[i].x - pA[i].x);
                    double y = pA[i].y + progresoPaso * (pB[i].y - pA[i].y);
                    temp.push_back({ x, y });
                }

                *verticesTransformados = temp;
                canvasBox->Invalidate();
            }
        }

        System::Void btnAbrirCreadorFrames_Click(System::Object^ sender, System::EventArgs^ e) {
            timerAnimacion->Stop();
            FormCreadorAnimacion^ frm = gcnew FormCreadorAnimacion();

            if (verticesTransformados != nullptr && !verticesTransformados->empty()) {
                frm->SetVerticesPersonalizados(*verticesTransformados);
            }
            else if (verticesOriginales != nullptr && !verticesOriginales->empty()) {
                frm->SetVerticesPersonalizados(*verticesOriginales);
            }

            if (frm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                *secuenciaAnimacionSecundaria = frm->GetFramesConfigurados();
                if (!secuenciaAnimacionSecundaria->empty()) {
                    modoAnimacionPorFrames = true;
                    lstPasos->Items->Clear();
                    for (size_t i = 0; i < secuenciaAnimacionSecundaria->size(); ++i) {
                        lstPasos->Items->Add(L"Frame " + (i + 1).ToString() + L" (Guardado)");
                    }
                    pasoAnimacionActual = 0;
                    progresoPaso = 0.0f;
                    lstPasos->SelectedIndex = 0;
                    *verticesTransformados = (*secuenciaAnimacionSecundaria)[0].verticesResultado;
                    canvasBox->Invalidate();
                }
                else {
                    modoAnimacionPorFrames = false;
                }
            }
            else {
                modoAnimacionPorFrames = false;
            }

            delete frm;
        }

        System::Void canvasBox_Resize(System::Object^ sender, System::EventArgs^ e) {
            canvasBox->Invalidate();
        }

        System::Void canvasBox_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->Clear(Color::FromArgb(5, 5, 5));

            if (imagenFondo != nullptr) {
                float opacidad = 0.15f;

                cli::array<cli::array<float>^>^ elementosMatriz = {
                    gcnew cli::array<float>{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
                    gcnew cli::array<float>{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
                    gcnew cli::array<float>{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
                    gcnew cli::array<float>{ 0.0f, 0.0f, 0.0f, opacidad, 0.0f },
                    gcnew cli::array<float>{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
                };

                ColorMatrix^ matrizColor = gcnew ColorMatrix(elementosMatriz);
                ImageAttributes^ atributosImagen = gcnew ImageAttributes();
                atributosImagen->SetColorMatrix(matrizColor, ColorMatrixFlag::Default, ColorAdjustType::Bitmap);

                System::Drawing::Rectangle destRect(0, 0, canvasBox->Width, canvasBox->Height);
                g->DrawImage(
                    imagenFondo,
                    destRect,
                    0, 0, imagenFondo->Width, imagenFondo->Height,
                    GraphicsUnit::Pixel,
                    atributosImagen
                );

                delete atributosImagen;
                delete matrizColor;
            }

            int centroX = canvasBox->Width / 2;
            int centroY = canvasBox->Height / 2;

            Pen^ penEjes = gcnew Pen(Color::FromArgb(57, 255, 20), 1.5f);
            g->DrawLine(penEjes, 0, centroY, canvasBox->Width, centroY);
            g->DrawLine(penEjes, centroX, 0, centroX, canvasBox->Height);
            delete penEjes;

            if (verticesTransformados->empty()) return;

            Pen^ penFigura = gcnew Pen(Color::FromArgb(0, 240, 255), 3.0f);
            for (size_t i = 0; i < verticesTransformados->size() - 1; ++i) {
                int x1 = centroX + (int)std::round((*verticesTransformados)[i].x);
                int y1 = centroY - (int)std::round((*verticesTransformados)[i].y);
                int x2 = centroX + (int)std::round((*verticesTransformados)[i + 1].x);
                int y2 = centroY - (int)std::round((*verticesTransformados)[i + 1].y);

                g->DrawLine(penFigura, x1, y1, x2, y2);
            }
            delete penFigura;
        }

        System::Void CargarFiguraBase() {
            modoAnimacionPorFrames = false;
            int opcion = cmbObjetos->SelectedIndex;

            if (opcion != 4) {
                verticesOriginales->clear();
            }

            switch (opcion) {
            case 0:
                *verticesOriginales = { {0, 0}, {100, 100} };
                break;
            case 1:
                *verticesOriginales = { {0,0}, {60,0}, {60,50}, {30,80}, {0,50}, {0,0} };
                break;
            case 2:
                *verticesOriginales = { {-50,0}, {50,0}, {50,15}, {30,15}, {20,35}, {-20,35}, {-30,15}, {-50,15}, {-50,0} };
                break;
            case 3:
                *verticesOriginales = {
                    {-120, 0}, {-120, 60}, {40, 60}, {40, 150}, {55, 150},
                    {55, 130}, {80, 130}, {80, 110}, {120, 110}, {120, 75},
                    {80, 60}, {80, 0}, {55, 0}, {55, 30}, {30, 10},
                    {0, 10}, {-90, 30}, {-90, 0}, {-120, 0}
                };
                break;
            case 4:
                break;
            }

            contadorPuntos = (int)verticesOriginales->size();
            lblContadorPuntos->Text = L"Puntos agregados: " + contadorPuntos.ToString();

            *verticesTransformados = *verticesOriginales;
            historialPasos->clear();
            lstPasos->Items->Clear();
            timerAnimacion->Stop();
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
                modoAnimacionPorFrames = false;
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
            timerAnimacion->Stop();
            modoAnimacionPorFrames = false;

            verticesOriginales->clear();
            verticesOriginales->shrink_to_fit();

            verticesTransformados->clear();
            verticesTransformados->shrink_to_fit();

            historialPasos->clear();
            historialPasos->shrink_to_fit();

            secuenciaAnimacionSecundaria->clear();
            secuenciaAnimacionSecundaria->shrink_to_fit();

            lstPasos->Items->Clear();
            contadorPuntos = 0;
            lblContadorPuntos->Text = L"Puntos agregados: 0";
            canvasBox->Invalidate();
        }

        System::Void btnPoligonoAleatorio_Click(System::Object^ sender, System::EventArgs^ e) {
            modoAnimacionPorFrames = false;
            verticesOriginales->clear();

            int n = 5 + (rand() % 6);
            for (int i = 0; i < n; ++i) {
                double x = (rand() % 201) - 100;
                double y = (rand() % 201) - 100;
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

        System::Void btnTransformar_Click(System::Object^ sender, System::EventArgs^ e) {
            modoAnimacionPorFrames = false;
            try {
                double tx = Convert::ToDouble(txtTx->Text);
                double ty = Convert::ToDouble(txtTy->Text);
                double sx = Convert::ToDouble(txtSx->Text);
                double sy = Convert::ToDouble(txtSy->Text);
                double ang = Convert::ToDouble(txtAngulo->Text);
                double shx = Convert::ToDouble(txtShx->Text);
                double shy = Convert::ToDouble(txtShy->Text);
                bool refX = chkReflejoX->Checked;
                bool refY = chkReflejoY->Checked;

                historialPasos->clear();
                lstPasos->Items->Clear();

                std::vector<Punto2D> estActual = *verticesOriginales;

                // PASO 1: ESCALA
                if (sx != 1.0 || sy != 1.0) {
                    for (auto& pt : estActual) { pt.x *= sx; pt.y *= sy; }
                    historialPasos->push_back({ ESCALA, "Paso 1: Escala (Sx=" + std::to_string(sx) + ", Sy=" + std::to_string(sy) + ")", estActual });
                    lstPasos->Items->Add(L"Paso 1: Escala");
                }

                // PASO 2: REFLEJO
                if (refX || refY) {
                    for (auto& pt : estActual) {
                        if (refY) pt.x *= -1.0;
                        if (refX) pt.y *= -1.0;
                    }
                    historialPasos->push_back({ REFLEXION, "Paso 2: Reflejo (X=" + std::to_string(refX) + ", Y=" + std::to_string(refY) + ")", estActual });
                    lstPasos->Items->Add(L"Paso 2: Reflejo");
                }

                // PASO 3: CIZALLAMIENTO
                if (shx != 0.0 || shy != 0.0) {
                    for (auto& pt : estActual) {
                        double nx = pt.x + shx * pt.y;
                        double ny = pt.y + shy * pt.x;
                        pt.x = nx; pt.y = ny;
                    }
                    historialPasos->push_back({ CIZALLAMIENTO, "Paso 3: Cizallamiento (Shx=" + std::to_string(shx) + ", Shy=" + std::to_string(shy) + ")", estActual });
                    lstPasos->Items->Add(L"Paso 3: Cizallamiento");
                }

                // PASO 4: ROTACIÓN
                if (ang != 0.0) {
                    double rad = ang * 3.14159265358979323846 / 180.0;
                    for (auto& pt : estActual) {
                        double nx = pt.x * cos(rad) - pt.y * sin(rad);
                        double ny = pt.x * sin(rad) + pt.y * cos(rad);
                        pt.x = nx; pt.y = ny;
                    }
                    historialPasos->push_back({ ROTACION, "Paso 4: Rotación (" + std::to_string(ang) + "°)", estActual });
                    lstPasos->Items->Add(L"Paso 4: Rotación");
                }

                // PASO 5: TRASLACIÓN
                if (tx != 0.0 || ty != 0.0 || historialPasos->empty()) {
                    for (auto& pt : estActual) { pt.x += tx; pt.y += ty; }
                    historialPasos->push_back({ TRASLACION, "Paso 5: Traslación (Tx=" + std::to_string(tx) + ", Ty=" + std::to_string(ty) + ")", estActual });
                    lstPasos->Items->Add(L"Paso final: Traslación");
                }

                *verticesTransformados = estActual;
                if (!lstPasos->Items->Count == 0) lstPasos->SelectedIndex = lstPasos->Items->Count - 1;
                canvasBox->Invalidate();
            }
            catch (...) {
                MessageBox::Show(L"Ingrese valores numéricos válidos.");
            }
        }

        System::Void btnReiniciarTransformaciones_Click(System::Object^ sender, System::EventArgs^ e) {
            timerAnimacion->Stop();
            txtTx->Text = L"0"; txtTy->Text = L"0";
            txtSx->Text = L"1"; txtSy->Text = L"1";
            txtAngulo->Text = L"0";
            txtShx->Text = L"0"; txtShy->Text = L"0";
            chkReflejoX->Checked = false;
            chkReflejoY->Checked = false;

            historialPasos->clear();
            lstPasos->Items->Clear();

            *verticesTransformados = *verticesOriginales;
            canvasBox->Invalidate();
        }

        System::Void btnAnimarPasos_Click(System::Object^ sender, System::EventArgs^ e) {
            if (modoAnimacionPorFrames) {
                if (!secuenciaAnimacionSecundaria->empty()) {
                    pasoAnimacionActual = 0;
                    progresoPaso = 0.0f;
                    timerAnimacion->Start();
                }
            }
            else {
                btnTransformar_Click(sender, e);
                if (historialPasos != nullptr && !historialPasos->empty()) {
                    pasoAnimacionActual = 0;
                    progresoPaso = 0.0f;
                    timerAnimacion->Start();
                }
            }
        }

        System::Void lstPasos_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
            int idx = lstPasos->SelectedIndex;
            if (modoAnimacionPorFrames && secuenciaAnimacionSecundaria != nullptr) {
                if (idx >= 0 && idx < (int)secuenciaAnimacionSecundaria->size()) {
                    *verticesTransformados = (*secuenciaAnimacionSecundaria)[idx].verticesResultado;
                    canvasBox->Invalidate();
                }
            }
            else if (historialPasos != nullptr) {
                if (idx >= 0 && idx < (int)historialPasos->size()) {
                    *verticesTransformados = (*historialPasos)[idx].verticesEstado;
                    canvasBox->Invalidate();
                }
            }
        }

        System::Void btnCerrar_Click(System::Object^ sender, System::EventArgs^ e) {
            this->Close();
        }
    };
}