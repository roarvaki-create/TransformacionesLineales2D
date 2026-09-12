#pragma once

#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace TransformacionesLineales2D {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    struct Punto2D {
        double x, y;
    };

    public ref class MyForm : public System::Windows::Forms::Form
    {
    private:
        std::vector<Punto2D>* verticesOriginales;
        std::vector<Punto2D>* verticesTransformados;
        int contadorPuntos;

        // Componentes GUI
        System::Windows::Forms::ComboBox^ cmbObjetos;
        System::Windows::Forms::Button^ btnCargar;
        System::Windows::Forms::Button^ btnSpawnAleatorio;
        System::Windows::Forms::Button^ btnTransformar;
        System::Windows::Forms::Button^ btnPoligonoAleatorio;
        System::Windows::Forms::Button^ btnCerrar;
        System::Windows::Forms::Panel^ canvasBox;

        // Coordenadas Manuales
        System::Windows::Forms::TextBox^ txtCoordX;
        System::Windows::Forms::TextBox^ txtCoordY;
        System::Windows::Forms::Button^ btnAgregarPunto;
        System::Windows::Forms::Button^ btnLimpiarPuntos;
        System::Windows::Forms::Label^ lblCoordX;
        System::Windows::Forms::Label^ lblCoordY;
        System::Windows::Forms::Label^ lblContadorPuntos;

        // Cajas de Texto de Transformaciones
        System::Windows::Forms::TextBox^ txtTx;
        System::Windows::Forms::TextBox^ txtTy;
        System::Windows::Forms::TextBox^ txtSx;
        System::Windows::Forms::TextBox^ txtSy;
        System::Windows::Forms::TextBox^ txtAngulo;
        System::Windows::Forms::TextBox^ txtShx;
        System::Windows::Forms::TextBox^ txtShy;

        // Etiquetas
        System::Windows::Forms::Label^ lblTx;
        System::Windows::Forms::Label^ lblTy;
        System::Windows::Forms::Label^ lblSx;
        System::Windows::Forms::Label^ lblSy;
        System::Windows::Forms::Label^ lblAngulo;
        System::Windows::Forms::Label^ lblShx;
        System::Windows::Forms::Label^ lblShy;

    public:
        MyForm(void)
        {
            InitializeComponent();
            srand((unsigned int)time(NULL));
            verticesOriginales = new std::vector<Punto2D>();
            verticesTransformados = new std::vector<Punto2D>();
            contadorPuntos = 0;
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
            // NOTA: Se eliminaron los delete de los punteros std::vector 
            // para evitar la excepción AccessViolationException al cerrar.
        }

    private:
        System::ComponentModel::Container^ components;

        void InitializeComponent(void)
        {
            this->cmbObjetos = (gcnew System::Windows::Forms::ComboBox());
            this->btnCargar = (gcnew System::Windows::Forms::Button());
            this->btnSpawnAleatorio = (gcnew System::Windows::Forms::Button());
            this->btnPoligonoAleatorio = (gcnew System::Windows::Forms::Button());
            this->btnTransformar = (gcnew System::Windows::Forms::Button());
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

            this->lblTx = (gcnew System::Windows::Forms::Label());
            this->lblTy = (gcnew System::Windows::Forms::Label());
            this->lblSx = (gcnew System::Windows::Forms::Label());
            this->lblSy = (gcnew System::Windows::Forms::Label());
            this->lblAngulo = (gcnew System::Windows::Forms::Label());
            this->lblShx = (gcnew System::Windows::Forms::Label());
            this->lblShy = (gcnew System::Windows::Forms::Label());

            this->Text = L"Programa para graficar las transformaciones lineales";
            this->Size = System::Drawing::Size(880, 720);

            this->canvasBox->Location = System::Drawing::Point(280, 20);
            this->canvasBox->Size = System::Drawing::Size(560, 620);
            this->canvasBox->BackColor = System::Drawing::Color::White;
            this->canvasBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::canvasBox_Paint);

            this->cmbObjetos->FormattingEnabled = true;
            this->cmbObjetos->Items->AddRange(gcnew cli::array< System::Object^ >(5) {
                L"Línea (0,0)", L"Casa", L"Auto", L"Perro", L"Personalizado"
            });
            this->cmbObjetos->Location = System::Drawing::Point(20, 20);
            this->cmbObjetos->Size = System::Drawing::Size(230, 21);
            this->cmbObjetos->SelectedIndex = 0;

            this->btnCargar->Location = System::Drawing::Point(20, 50);
            this->btnCargar->Size = System::Drawing::Size(110, 25);
            this->btnCargar->Text = L"Cargar Objeto";
            this->btnCargar->Click += gcnew System::EventHandler(this, &MyForm::btnCargar_Click);

            this->btnSpawnAleatorio->Location = System::Drawing::Point(140, 50);
            this->btnSpawnAleatorio->Size = System::Drawing::Size(110, 25);
            this->btnSpawnAleatorio->Text = L"Spawn Aleatorio";
            this->btnSpawnAleatorio->Click += gcnew System::EventHandler(this, &MyForm::btnSpawnAleatorio_Click);

            this->lblCoordX->Text = L"X:";
            this->lblCoordX->Location = System::Drawing::Point(20, 90);
            this->lblCoordX->AutoSize = true;
            this->txtCoordX->Text = L"0";
            this->txtCoordX->Location = System::Drawing::Point(40, 87);
            this->txtCoordX->Size = System::Drawing::Size(40, 20);

            this->lblCoordY->Text = L"Y:";
            this->lblCoordY->Location = System::Drawing::Point(85, 90);
            this->lblCoordY->AutoSize = true;
            this->txtCoordY->Text = L"0";
            this->txtCoordY->Location = System::Drawing::Point(105, 87);
            this->txtCoordY->Size = System::Drawing::Size(40, 20);

            this->btnAgregarPunto->Location = System::Drawing::Point(150, 85);
            this->btnAgregarPunto->Size = System::Drawing::Size(100, 25);
            this->btnAgregarPunto->Text = L"Agregar Punto";
            this->btnAgregarPunto->Click += gcnew System::EventHandler(this, &MyForm::btnAgregarPunto_Click);

            this->lblContadorPuntos->Text = L"Puntos agregados: 0";
            this->lblContadorPuntos->Location = System::Drawing::Point(20, 115);
            this->lblContadorPuntos->AutoSize = true;

            this->btnLimpiarPuntos->Location = System::Drawing::Point(20, 135);
            this->btnLimpiarPuntos->Size = System::Drawing::Size(230, 25);
            this->btnLimpiarPuntos->Text = L"Limpiar Coordenadas Manuales";
            this->btnLimpiarPuntos->Click += gcnew System::EventHandler(this, &MyForm::btnLimpiarPuntos_Click);

            this->btnPoligonoAleatorio->Location = System::Drawing::Point(20, 165);
            this->btnPoligonoAleatorio->Size = System::Drawing::Size(230, 25);
            this->btnPoligonoAleatorio->Text = L"Generar Puntos Aleatorios";
            this->btnPoligonoAleatorio->Click += gcnew System::EventHandler(this, &MyForm::btnPoligonoAleatorio_Click);

            this->lblTx->Text = L"Traslación X (Tx):";
            this->lblTx->Location = System::Drawing::Point(20, 210);
            this->lblTx->AutoSize = true;
            this->txtTx->Text = L"0";
            this->txtTx->Location = System::Drawing::Point(150, 207);
            this->txtTx->Size = System::Drawing::Size(100, 20);

            this->lblTy->Text = L"Traslación Y (Ty):";
            this->lblTy->Location = System::Drawing::Point(20, 240);
            this->lblTy->AutoSize = true;
            this->txtTy->Text = L"0";
            this->txtTy->Location = System::Drawing::Point(150, 237);
            this->txtTy->Size = System::Drawing::Size(100, 20);

            this->lblSx->Text = L"Escala X (Sx):";
            this->lblSx->Location = System::Drawing::Point(20, 270);
            this->lblSx->AutoSize = true;
            this->txtSx->Text = L"1";
            this->txtSx->Location = System::Drawing::Point(150, 267);
            this->txtSx->Size = System::Drawing::Size(100, 20);

            this->lblSy->Text = L"Escala Y (Sy):";
            this->lblSy->Location = System::Drawing::Point(20, 300);
            this->lblSy->AutoSize = true;
            this->txtSy->Text = L"1";
            this->txtSy->Location = System::Drawing::Point(150, 297);
            this->txtSy->Size = System::Drawing::Size(100, 20);

            this->lblAngulo->Text = L"Ángulo (°):";
            this->lblAngulo->Location = System::Drawing::Point(20, 330);
            this->lblAngulo->AutoSize = true;
            this->txtAngulo->Text = L"0";
            this->txtAngulo->Location = System::Drawing::Point(150, 327);
            this->txtAngulo->Size = System::Drawing::Size(100, 20);

            this->lblShx->Text = L"Cizallamiento X:";
            this->lblShx->Location = System::Drawing::Point(20, 360);
            this->lblShx->AutoSize = true;
            this->txtShx->Text = L"0";
            this->txtShx->Location = System::Drawing::Point(150, 357);
            this->txtShx->Size = System::Drawing::Size(100, 20);

            this->lblShy->Text = L"Cizallamiento Y:";
            this->lblShy->Location = System::Drawing::Point(20, 390);
            this->lblShy->AutoSize = true;
            this->txtShy->Text = L"0";
            this->txtShy->Location = System::Drawing::Point(150, 387);
            this->txtShy->Size = System::Drawing::Size(100, 20);

            this->btnTransformar->Location = System::Drawing::Point(20, 430);
            this->btnTransformar->Size = System::Drawing::Size(230, 35);
            this->btnTransformar->Text = L"Transformar";
            this->btnTransformar->Click += gcnew System::EventHandler(this, &MyForm::btnTransformar_Click);

            this->btnCerrar->Location = System::Drawing::Point(20, 605);
            this->btnCerrar->Size = System::Drawing::Size(230, 35);
            this->btnCerrar->Text = L"Cerrar Aplicación";
            this->btnCerrar->BackColor = System::Drawing::Color::LightCoral;
            this->btnCerrar->Click += gcnew System::EventHandler(this, &MyForm::btnCerrar_Click);

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

            this->Controls->Add(this->btnTransformar);
            this->Controls->Add(this->btnCerrar);
        }

        Punto2D AplicarMatriz(double M[3][3], Punto2D p) {
            Punto2D res;
            res.x = M[0][0] * p.x + M[0][1] * p.y + M[0][2] * 1.0;
            res.y = M[1][0] * p.x + M[1][1] * p.y + M[1][2] * 1.0;
            return res;
        }

    private:
        System::Void canvasBox_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->Clear(Color::White);

            int centroX = canvasBox->Width / 2;
            int centroY = canvasBox->Height / 2;

            Pen^ penEjes = gcnew Pen(Color::LightGray, 1.0f);
            g->DrawLine(penEjes, 0, centroY, canvasBox->Width, centroY);
            g->DrawLine(penEjes, centroX, 0, centroX, canvasBox->Height);

            if (verticesTransformados->empty()) return;

            Pen^ penFigura = gcnew Pen(Color::Blue, 2.0f);
            for (size_t i = 0; i < verticesTransformados->size() - 1; ++i) {
                int x1 = centroX + (int)(*verticesTransformados)[i].x;
                int y1 = centroY - (int)(*verticesTransformados)[i].y;
                int x2 = centroX + (int)(*verticesTransformados)[i + 1].x;
                int y2 = centroY - (int)(*verticesTransformados)[i + 1].y;

                g->DrawLine(penFigura, x1, y1, x2, y2);
            }
        }

        System::Void CargarFiguraBase() {
            int opcion = cmbObjetos->SelectedIndex;

            if (opcion != 4) {
                verticesOriginales->clear();
                contadorPuntos = 0;
                lblContadorPuntos->Text = L"Puntos agregados: 0";
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

            *verticesTransformados = *verticesOriginales;
        }

        System::Void btnCargar_Click(System::Object^ sender, System::EventArgs^ e) {
            CargarFiguraBase();
            canvasBox->Invalidate();
        }

        System::Void btnSpawnAleatorio_Click(System::Object^ sender, System::EventArgs^ e) {
            CargarFiguraBase();
            if (verticesOriginales->empty()) return;

            double randTx = (rand() % 301) - 150;
            double randTy = (rand() % 301) - 150;

            for (auto& p : *verticesOriginales) {
                p.x += randTx;
                p.y += randTy;
            }

            *verticesTransformados = *verticesOriginales;
            canvasBox->Invalidate();
        }

        System::Void btnAgregarPunto_Click(System::Object^ sender, System::EventArgs^ e) {
            try {
                double x = Convert::ToDouble(txtCoordX->Text);
                double y = Convert::ToDouble(txtCoordY->Text);

                // Añade el nuevo punto a los vértices que ya existen
                verticesOriginales->push_back({ x, y });
                contadorPuntos++;

                lblContadorPuntos->Text = L"Puntos agregados: " + contadorPuntos.ToString();

                // Cambia la selección visual a Personalizado sin borrar lo anterior
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

                double M[3][3] = {
                    {sx * cos(rad), -sy * sin(rad) + shx, tx},
                    {sx * sin(rad) + shy, sy * cos(rad), ty},
                    {0, 0, 1}
                };

                verticesTransformados->clear();
                for (const auto& p : *verticesOriginales) {
                    verticesTransformados->push_back(AplicarMatriz(M, p));
                }

                canvasBox->Invalidate();
            }
            catch (...) {
                MessageBox::Show(L"Revise los campos numéricos de las transformaciones.");
            }
        }

        System::Void btnCerrar_Click(System::Object^ sender, System::EventArgs^ e) {
            this->Close();
        }
    };
}