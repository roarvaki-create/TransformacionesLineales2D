#pragma once

#include "Punto2D.h"
#include "DoubleBufferedPanel.h"

#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

namespace TransformacionesLineales2D {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Drawing::Imaging;

    public struct ConfiguracionFrame {
        int numeroFrame;
        bool acumularDesdeAnterior;
        bool animarTransicion;

        double origenX, origenY;
        double tx, ty;
        double sx, sy;
        double angulo;
        double shx, shy;
        bool reflejoX, reflejoY;

        std::vector<Punto2D> verticesResultado;
    };

    public ref class FormCreadorAnimacion : public System::Windows::Forms::Form
    {
    private:
        std::vector<Punto2D>* verticesBase;
        std::vector<ConfiguracionFrame>* listaFrames;
        int frameActualIndex;
        int totalFrames;

        // GUI
        System::Windows::Forms::ComboBox^ cmbObjetos;
        System::Windows::Forms::NumericUpDown^ numTotalFrames;
        System::Windows::Forms::Button^ btnCrearFrames;

        System::Windows::Forms::Label^ lblFrameActual;
        System::Windows::Forms::CheckBox^ chkAcumular;
        System::Windows::Forms::CheckBox^ chkAnimarTransicion;

        System::Windows::Forms::TextBox^ txtOrigenX;
        System::Windows::Forms::TextBox^ txtOrigenY;
        System::Windows::Forms::TextBox^ txtTx;
        System::Windows::Forms::TextBox^ txtTy;
        System::Windows::Forms::TextBox^ txtSx;
        System::Windows::Forms::TextBox^ txtSy;
        System::Windows::Forms::TextBox^ txtAngulo;
        System::Windows::Forms::TextBox^ txtShx;
        System::Windows::Forms::TextBox^ txtShy;
        System::Windows::Forms::CheckBox^ chkReflejoX;
        System::Windows::Forms::CheckBox^ chkReflejoY;

        System::Windows::Forms::Label^ lblOrigenX;
        System::Windows::Forms::Label^ lblOrigenY;
        System::Windows::Forms::Label^ lblTx;
        System::Windows::Forms::Label^ lblTy;
        System::Windows::Forms::Label^ lblSx;
        System::Windows::Forms::Label^ lblSy;
        System::Windows::Forms::Label^ lblAngulo;
        System::Windows::Forms::Label^ lblShx;
        System::Windows::Forms::Label^ lblShy;

        System::Windows::Forms::Button^ btnAnterior;
        System::Windows::Forms::Button^ btnSiguiente;
        System::Windows::Forms::Button^ btnFinalizar;

        DoubleBufferedPanel^ previewCanvas;

    public:
        void SetVerticesPersonalizados(const std::vector<Punto2D>& puntos) {
            if (verticesBase != nullptr) {
                *verticesBase = puntos;
                InicializarEstructuraFrames(totalFrames);
            }
        }
        FormCreadorAnimacion(void)
        {
            InitializeComponent();
            verticesBase = new std::vector<Punto2D>();
            listaFrames = new std::vector<ConfiguracionFrame>();
            frameActualIndex = 0;
            totalFrames = 3;
            CargarFiguraDefault(0);
            InicializarEstructuraFrames(3);
        }

        std::vector<ConfiguracionFrame> GetFramesConfigurados() {
            if (listaFrames != nullptr) return *listaFrames;
            return std::vector<ConfiguracionFrame>();
        }

    protected:
        ~FormCreadorAnimacion()
        {
            if (components) delete components;

            if (verticesBase != nullptr) {
                verticesBase->clear();
                verticesBase->shrink_to_fit();
                delete verticesBase;
                verticesBase = nullptr;
            }
            if (listaFrames != nullptr) {
                for (size_t i = 0; i < listaFrames->size(); ++i) {
                    (*listaFrames)[i].verticesResultado.clear();
                    (*listaFrames)[i].verticesResultado.shrink_to_fit();
                }
                listaFrames->clear();
                listaFrames->shrink_to_fit();
                delete listaFrames;
                listaFrames = nullptr;
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
            this->cmbObjetos = (gcnew System::Windows::Forms::ComboBox());
            this->numTotalFrames = (gcnew System::Windows::Forms::NumericUpDown());
            this->btnCrearFrames = (gcnew System::Windows::Forms::Button());
            this->lblFrameActual = (gcnew System::Windows::Forms::Label());
            this->chkAcumular = (gcnew System::Windows::Forms::CheckBox());
            this->chkAnimarTransicion = (gcnew System::Windows::Forms::CheckBox());

            this->txtOrigenX = (gcnew System::Windows::Forms::TextBox());
            this->txtOrigenY = (gcnew System::Windows::Forms::TextBox());
            this->txtTx = (gcnew System::Windows::Forms::TextBox());
            this->txtTy = (gcnew System::Windows::Forms::TextBox());
            this->txtSx = (gcnew System::Windows::Forms::TextBox());
            this->txtSy = (gcnew System::Windows::Forms::TextBox());
            this->txtAngulo = (gcnew System::Windows::Forms::TextBox());
            this->txtShx = (gcnew System::Windows::Forms::TextBox());
            this->txtShy = (gcnew System::Windows::Forms::TextBox());

            this->chkReflejoX = (gcnew System::Windows::Forms::CheckBox());
            this->chkReflejoY = (gcnew System::Windows::Forms::CheckBox());

            this->lblOrigenX = (gcnew System::Windows::Forms::Label());
            this->lblOrigenY = (gcnew System::Windows::Forms::Label());
            this->lblTx = (gcnew System::Windows::Forms::Label());
            this->lblTy = (gcnew System::Windows::Forms::Label());
            this->lblSx = (gcnew System::Windows::Forms::Label());
            this->lblSy = (gcnew System::Windows::Forms::Label());
            this->lblAngulo = (gcnew System::Windows::Forms::Label());
            this->lblShx = (gcnew System::Windows::Forms::Label());
            this->lblShy = (gcnew System::Windows::Forms::Label());

            this->btnAnterior = (gcnew System::Windows::Forms::Button());
            this->btnSiguiente = (gcnew System::Windows::Forms::Button());
            this->btnFinalizar = (gcnew System::Windows::Forms::Button());
            this->previewCanvas = (gcnew DoubleBufferedPanel());

            this->SuspendLayout();

            this->BackColor = Color::FromArgb(18, 18, 18);
            this->ClientSize = System::Drawing::Size(750, 520);
            this->Text = L"Creador de Animaciones por Frames";

            // Combo Figura
            this->cmbObjetos->FormattingEnabled = true;
            this->cmbObjetos->Items->AddRange(gcnew cli::array< System::Object^ >(4) { L"Línea (0,0)", L"Casa", L"Auto", L"Perro" });
            this->cmbObjetos->Location = System::Drawing::Point(20, 20);
            this->cmbObjetos->Size = System::Drawing::Size(130, 21);
            this->cmbObjetos->SelectedIndex = 1;
            this->cmbObjetos->BackColor = Color::FromArgb(45, 45, 48);
            this->cmbObjetos->ForeColor = Color::White;
            this->cmbObjetos->SelectedIndexChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::cmbObjetos_SelectedIndexChanged);

            // Num Total Frames
            this->numTotalFrames->Location = System::Drawing::Point(160, 20);
            this->numTotalFrames->Size = System::Drawing::Size(50, 20);
            this->numTotalFrames->Minimum = 1;
            this->numTotalFrames->Maximum = 20;
            this->numTotalFrames->Value = 3;
            this->numTotalFrames->BackColor = Color::FromArgb(45, 45, 48);
            this->numTotalFrames->ForeColor = Color::White;

            this->btnCrearFrames->Location = System::Drawing::Point(220, 18);
            this->btnCrearFrames->Size = System::Drawing::Size(100, 25);
            this->btnCrearFrames->Text = L"Set Total Frames";
            EstilarBoton(this->btnCrearFrames, Color::FromArgb(40, 100, 180), Color::White);
            this->btnCrearFrames->Click += gcnew System::EventHandler(this, &FormCreadorAnimacion::btnCrearFrames_Click);

            // Label Frame Actual
            this->lblFrameActual->Location = System::Drawing::Point(20, 55);
            this->lblFrameActual->Size = System::Drawing::Size(300, 20);
            this->lblFrameActual->Text = L"FRAME 1 DE 3";
            this->lblFrameActual->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->lblFrameActual->ForeColor = Color::FromArgb(0, 240, 255);

            // Checkboxes de Modos
            this->chkAcumular->Location = System::Drawing::Point(20, 80);
            this->chkAcumular->Size = System::Drawing::Size(220, 20);
            this->chkAcumular->Text = L"Acumular desde frame anterior";
            this->chkAcumular->ForeColor = Color::White;
            this->chkAcumular->CheckedChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::chkAcumular_CheckedChanged);

            this->chkAnimarTransicion->Location = System::Drawing::Point(20, 100);
            this->chkAnimarTransicion->Size = System::Drawing::Size(220, 20);
            this->chkAnimarTransicion->Text = L"Animar transición hacia este frame";
            this->chkAnimarTransicion->Checked = true;
            this->chkAnimarTransicion->ForeColor = Color::FromArgb(57, 255, 20);

            // Inputs
            int yPos = 130;
            this->lblOrigenX->Location = System::Drawing::Point(20, yPos); this->lblOrigenX->Text = L"Origen Manual X:"; EstilarLabel(this->lblOrigenX);
            this->txtOrigenX->Location = System::Drawing::Point(140, yPos); this->txtOrigenX->Size = System::Drawing::Size(80, 20); this->txtOrigenX->Text = L"0"; EstilarTextBox(this->txtOrigenX); yPos += 28;

            this->lblOrigenY->Location = System::Drawing::Point(20, yPos); this->lblOrigenY->Text = L"Origen Manual Y:"; EstilarLabel(this->lblOrigenY);
            this->txtOrigenY->Location = System::Drawing::Point(140, yPos); this->txtOrigenY->Size = System::Drawing::Size(80, 20); this->txtOrigenY->Text = L"0"; EstilarTextBox(this->txtOrigenY); yPos += 28;

            this->lblTx->Location = System::Drawing::Point(20, yPos); this->lblTx->Text = L"Traslación X (Tx):"; EstilarLabel(this->lblTx);
            this->txtTx->Location = System::Drawing::Point(140, yPos); this->txtTx->Size = System::Drawing::Size(80, 20); this->txtTx->Text = L"0"; EstilarTextBox(this->txtTx); yPos += 28;

            this->lblTy->Location = System::Drawing::Point(20, yPos); this->lblTy->Text = L"Traslación Y (Ty):"; EstilarLabel(this->lblTy);
            this->txtTy->Location = System::Drawing::Point(140, yPos); this->txtTy->Size = System::Drawing::Size(80, 20); this->txtTy->Text = L"0"; EstilarTextBox(this->txtTy); yPos += 28;

            this->lblSx->Location = System::Drawing::Point(20, yPos); this->lblSx->Text = L"Escala X (Sx):"; EstilarLabel(this->lblSx);
            this->txtSx->Location = System::Drawing::Point(140, yPos); this->txtSx->Size = System::Drawing::Size(80, 20); this->txtSx->Text = L"1"; EstilarTextBox(this->txtSx); yPos += 28;

            this->lblSy->Location = System::Drawing::Point(20, yPos); this->lblSy->Text = L"Escala Y (Sy):"; EstilarLabel(this->lblSy);
            this->txtSy->Location = System::Drawing::Point(140, yPos); this->txtSy->Size = System::Drawing::Size(80, 20); this->txtSy->Text = L"1"; EstilarTextBox(this->txtSy); yPos += 28;

            this->lblAngulo->Location = System::Drawing::Point(20, yPos); this->lblAngulo->Text = L"Ángulo (°):"; EstilarLabel(this->lblAngulo);
            this->txtAngulo->Location = System::Drawing::Point(140, yPos); this->txtAngulo->Size = System::Drawing::Size(80, 20); this->txtAngulo->Text = L"0"; EstilarTextBox(this->txtAngulo); yPos += 28;

            this->lblShx->Location = System::Drawing::Point(20, yPos); this->lblShx->Text = L"Cizallamiento X:"; EstilarLabel(this->lblShx);
            this->txtShx->Location = System::Drawing::Point(140, yPos); this->txtShx->Size = System::Drawing::Size(80, 20); this->txtShx->Text = L"0"; EstilarTextBox(this->txtShx); yPos += 28;

            this->lblShy->Location = System::Drawing::Point(20, yPos); this->lblShy->Text = L"Cizallamiento Y:"; EstilarLabel(this->lblShy);
            this->txtShy->Location = System::Drawing::Point(140, yPos); this->txtShy->Size = System::Drawing::Size(80, 20); this->txtShy->Text = L"0"; EstilarTextBox(this->txtShy); yPos += 28;

            this->chkReflejoX->Location = System::Drawing::Point(20, yPos); this->chkReflejoX->Text = L"Reflejo X"; this->chkReflejoX->ForeColor = Color::White;
            this->chkReflejoY->Location = System::Drawing::Point(120, yPos); this->chkReflejoY->Text = L"Reflejo Y"; this->chkReflejoY->ForeColor = Color::White; yPos += 30;

            // Botones Navegación
            this->btnAnterior->Location = System::Drawing::Point(20, yPos);
            this->btnAnterior->Size = System::Drawing::Size(90, 30);
            this->btnAnterior->Text = L"◄ Anterior";
            EstilarBoton(this->btnAnterior, Color::FromArgb(60, 60, 65), Color::White);
            this->btnAnterior->Click += gcnew System::EventHandler(this, &FormCreadorAnimacion::btnAnterior_Click);

            this->btnSiguiente->Location = System::Drawing::Point(120, yPos);
            this->btnSiguiente->Size = System::Drawing::Size(90, 30);
            this->btnSiguiente->Text = L"Siguiente ►";
            EstilarBoton(this->btnSiguiente, Color::FromArgb(0, 150, 200), Color::White);
            this->btnSiguiente->Click += gcnew System::EventHandler(this, &FormCreadorAnimacion::btnSiguiente_Click);

            this->btnFinalizar->Location = System::Drawing::Point(220, yPos);
            this->btnFinalizar->Size = System::Drawing::Size(100, 30);
            this->btnFinalizar->Text = L"✔ Guardar";
            EstilarBoton(this->btnFinalizar, Color::FromArgb(0, 200, 100), Color::Black);
            this->btnFinalizar->Click += gcnew System::EventHandler(this, &FormCreadorAnimacion::btnFinalizar_Click);

            // Preview Canvas
            this->previewCanvas->Location = System::Drawing::Point(340, 20);
            this->previewCanvas->Size = System::Drawing::Size(380, 470);
            this->previewCanvas->BackColor = Color::FromArgb(5, 5, 5);
            this->previewCanvas->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &FormCreadorAnimacion::previewCanvas_Paint);

            // Eventos para actualizar la vista previa al escribir
            this->txtOrigenX->TextChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->txtOrigenY->TextChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->txtTx->TextChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->txtTy->TextChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->txtSx->TextChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->txtSy->TextChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->txtAngulo->TextChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->txtShx->TextChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->txtShy->TextChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->chkReflejoX->CheckedChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);
            this->chkReflejoY->CheckedChanged += gcnew System::EventHandler(this, &FormCreadorAnimacion::OnValoresChanged);

            this->Controls->Add(this->cmbObjetos);
            this->Controls->Add(this->numTotalFrames);
            this->Controls->Add(this->btnCrearFrames);
            this->Controls->Add(this->lblFrameActual);
            this->Controls->Add(this->chkAcumular);
            this->Controls->Add(this->chkAnimarTransicion);
            this->Controls->Add(this->lblOrigenX); this->Controls->Add(this->txtOrigenX);
            this->Controls->Add(this->lblOrigenY); this->Controls->Add(this->txtOrigenY);
            this->Controls->Add(this->lblTx); this->Controls->Add(this->txtTx);
            this->Controls->Add(this->lblTy); this->Controls->Add(this->txtTy);
            this->Controls->Add(this->lblSx); this->Controls->Add(this->txtSx);
            this->Controls->Add(this->lblSy); this->Controls->Add(this->txtSy);
            this->Controls->Add(this->lblAngulo); this->Controls->Add(this->txtAngulo);
            this->Controls->Add(this->lblShx); this->Controls->Add(this->txtShx);
            this->Controls->Add(this->lblShy); this->Controls->Add(this->txtShy);
            this->Controls->Add(this->chkReflejoX); this->Controls->Add(this->chkReflejoY);
            this->Controls->Add(this->btnAnterior);
            this->Controls->Add(this->btnSiguiente);
            this->Controls->Add(this->btnFinalizar);
            this->Controls->Add(this->previewCanvas);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void CargarFiguraDefault(int index) {
            verticesBase->clear();
            switch (index) {
            case 0:
                verticesBase->push_back({ 0, 0 });
                verticesBase->push_back({ 80, 80 });
                break;
            case 1:
                verticesBase->push_back({ 0,0 }); verticesBase->push_back({ 50,0 });
                verticesBase->push_back({ 50,40 }); verticesBase->push_back({ 25,60 });
                verticesBase->push_back({ 0,40 }); verticesBase->push_back({ 0,0 });
                break;
            case 2:
                verticesBase->push_back({ -40,0 }); verticesBase->push_back({ 40,0 });
                verticesBase->push_back({ 40,10 }); verticesBase->push_back({ 20,10 });
                verticesBase->push_back({ 15,25 }); verticesBase->push_back({ -15,25 });
                verticesBase->push_back({ -20,10 }); verticesBase->push_back({ -40,10 });
                verticesBase->push_back({ -40,0 });
                break;
            case 3:
                verticesBase->push_back({ -80,0 }); verticesBase->push_back({ -80,40 });
                verticesBase->push_back({ 30,40 }); verticesBase->push_back({ 30,90 });
                verticesBase->push_back({ 40,90 }); verticesBase->push_back({ 50,70 });
                verticesBase->push_back({ 70,70 }); verticesBase->push_back({ 50,40 });
                verticesBase->push_back({ 50,0 }); verticesBase->push_back({ -80,0 });
                break;
            }
        }

        void InicializarEstructuraFrames(int n) {
            totalFrames = n;
            listaFrames->clear();
            for (int i = 0; i < n; ++i) {
                ConfiguracionFrame f;
                f.numeroFrame = i + 1;
                f.acumularDesdeAnterior = (i > 0);
                f.animarTransicion = true;
                f.origenX = 0; f.origenY = 0;
                f.tx = 0; f.ty = 0;
                f.sx = 1; f.sy = 1;
                f.angulo = 0;
                f.shx = 0; f.shy = 0;
                f.reflejoX = false; f.reflejoY = false;
                f.verticesResultado = *verticesBase;
                listaFrames->push_back(f);
            }
            frameActualIndex = 0;
            CargarValoresEnGUI();
        }

        void GuardarGUIEnFrameActual() {
            if (listaFrames == nullptr || frameActualIndex >= (int)listaFrames->size()) return;

            ConfiguracionFrame& f = (*listaFrames)[frameActualIndex];
            f.acumularDesdeAnterior = chkAcumular->Checked;
            f.animarTransicion = chkAnimarTransicion->Checked;

            try {
                f.origenX = Convert::ToDouble(txtOrigenX->Text);
                f.origenY = Convert::ToDouble(txtOrigenY->Text);
                f.tx = Convert::ToDouble(txtTx->Text);
                f.ty = Convert::ToDouble(txtTy->Text);
                f.sx = Convert::ToDouble(txtSx->Text);
                f.sy = Convert::ToDouble(txtSy->Text);
                f.angulo = Convert::ToDouble(txtAngulo->Text);
                f.shx = Convert::ToDouble(txtShx->Text);
                f.shy = Convert::ToDouble(txtShy->Text);
            }
            catch (...) {}

            f.reflejoX = chkReflejoX->Checked;
            f.reflejoY = chkReflejoY->Checked;

            f.verticesResultado = CalcularGeometriaFrame(frameActualIndex);
        }

        std::vector<Punto2D> CalcularGeometriaFrame(int index) {
            std::vector<Punto2D> res;
            if (listaFrames == nullptr || index < 0 || index >= static_cast<int>(listaFrames->size())) {
                if (verticesBase != nullptr) return *verticesBase;
                return res;
            }

            const ConfiguracionFrame& f = (*listaFrames)[index];

            const std::vector<Punto2D>* pEntrada = nullptr;
            if (index > 0 && f.acumularDesdeAnterior) {
                pEntrada = &((*listaFrames)[index - 1].verticesResultado);
            }
            else {
                pEntrada = verticesBase;
            }

            if (pEntrada == nullptr) return res;

            std::vector<Punto2D> pts = *pEntrada;

            // Convertir el ángulo a radianes
            double rad = f.angulo * 3.14159265358979323846 / 180.0;

            // Matrices de Transformación
            double S[2][2] = { {f.sx, 0.0}, {0.0, f.sy} };
            double Ref[2][2] = { {f.reflejoY ? -1.0 : 1.0, 0.0}, {0.0, f.reflejoX ? -1.0 : 1.0} };
            double Sh[2][2] = { {1.0, f.shx}, {f.shy, 1.0} };
            double R[2][2] = { {cos(rad), -sin(rad)}, {sin(rad), cos(rad)} };

            for (size_t i = 0; i < pts.size(); ++i) {
                Punto2D pt = pts[i];

                // 1. Aplicar el Origen Manual como Pivote de Rotación/Escala
                double px = pt.x + f.origenX;
                double py = pt.y + f.origenY;

                // 2. Aplicar Escala
                Punto2D p1 = { S[0][0] * px + S[0][1] * py, S[1][0] * px + S[1][1] * py };

                // 3. Aplicar Reflejo
                Punto2D p2 = { Ref[0][0] * p1.x + Ref[0][1] * p1.y, Ref[1][0] * p1.x + Ref[1][1] * p1.y };

                // 4. Aplicar Cizallamiento
                Punto2D p3 = { Sh[0][0] * p2.x + Sh[0][1] * p2.y, Sh[1][0] * p2.x + Sh[1][1] * p2.y };

                // 5. Aplicar Rotación sobre el pivote
                Punto2D p4 = { R[0][0] * p3.x + R[0][1] * p3.y, R[1][0] * p3.x + R[1][1] * p3.y };

                // 6. Aplicar Traslación Final (Tx, Ty)
                Punto2D pFinal = { p4.x + f.tx, p4.y + f.ty };

                res.push_back(pFinal);
            }

            return res;
        }

        void CargarValoresEnGUI() {
            if (listaFrames == nullptr || frameActualIndex >= (int)listaFrames->size()) return;

            const ConfiguracionFrame& f = (*listaFrames)[frameActualIndex];
            lblFrameActual->Text = L"FRAME " + (frameActualIndex + 1).ToString() + L" DE " + totalFrames.ToString();

            chkAcumular->Checked = f.acumularDesdeAnterior;
            chkAcumular->Enabled = (frameActualIndex > 0);
            chkAnimarTransicion->Checked = f.animarTransicion;

            txtOrigenX->Text = f.origenX.ToString();
            txtOrigenY->Text = f.origenY.ToString();
            txtTx->Text = f.tx.ToString();
            txtTy->Text = f.ty.ToString();
            txtSx->Text = f.sx.ToString();
            txtSy->Text = f.sy.ToString();
            txtAngulo->Text = f.angulo.ToString();
            txtShx->Text = f.shx.ToString();
            txtShy->Text = f.shy.ToString();

            chkReflejoX->Checked = f.reflejoX;
            chkReflejoY->Checked = f.reflejoY;

            previewCanvas->Invalidate();
        }

    private:
        System::Void cmbObjetos_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
            CargarFiguraDefault(cmbObjetos->SelectedIndex);
            InicializarEstructuraFrames(totalFrames);
        }

        System::Void btnCrearFrames_Click(System::Object^ sender, System::EventArgs^ e) {
            int n = (int)numTotalFrames->Value;
            InicializarEstructuraFrames(n);
        }

        System::Void chkAcumular_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
            GuardarGUIEnFrameActual();
            previewCanvas->Invalidate();
        }

        System::Void OnValoresChanged(System::Object^ sender, System::EventArgs^ e) {
            GuardarGUIEnFrameActual();
            previewCanvas->Invalidate();
        }

        System::Void btnAnterior_Click(System::Object^ sender, System::EventArgs^ e) {
            if (frameActualIndex > 0) {
                GuardarGUIEnFrameActual();
                frameActualIndex--;
                CargarValoresEnGUI();
            }
        }

        System::Void btnSiguiente_Click(System::Object^ sender, System::EventArgs^ e) {
            if (frameActualIndex < totalFrames - 1) {
                GuardarGUIEnFrameActual();
                frameActualIndex++;
                CargarValoresEnGUI();
            }
        }

        System::Void btnFinalizar_Click(System::Object^ sender, System::EventArgs^ e) {
            GuardarGUIEnFrameActual();
            for (size_t i = 0; i < listaFrames->size(); ++i) {
                (*listaFrames)[i].verticesResultado = CalcularGeometriaFrame((int)i);
            }
            this->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->Close();
        }

        System::Void previewCanvas_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->Clear(Color::FromArgb(5, 5, 5));

            // Cargar imagen de fondo de forma segura liberando recursos inmediatamente
            try {
                Image^ imgFondo = Image::FromFile("wha.png");
                if (imgFondo != nullptr) {
                    System::Drawing::Imaging::ColorMatrix^ matrix = gcnew System::Drawing::Imaging::ColorMatrix();
                    matrix->Matrix33 = 0.15f; // Opacidad

                    System::Drawing::Imaging::ImageAttributes^ attr = gcnew System::Drawing::Imaging::ImageAttributes();
                    attr->SetColorMatrix(matrix, System::Drawing::Imaging::ColorMatrixFlag::Default, System::Drawing::Imaging::ColorAdjustType::Bitmap);

                    Rectangle destRect = Rectangle(0, 0, previewCanvas->Width, previewCanvas->Height);
                    g->DrawImage(imgFondo, destRect, 0, 0, imgFondo->Width, imgFondo->Height, GraphicsUnit::Pixel, attr);

                    delete attr;
                    delete matrix;
                    delete imgFondo; // Evita el bloqueo del archivo en disco y la fuga de RAM
                }
            }
            catch (...) {
                // Manejo en caso de que wha.png no exista en el directorio de ejecución
            }

            int centroX = previewCanvas->Width / 2;
            int centroY = previewCanvas->Height / 2;

            Pen^ penEjes = gcnew Pen(Color::FromArgb(57, 255, 20), 1.0f);
            g->DrawLine(penEjes, 0, centroY, previewCanvas->Width, centroY);
            g->DrawLine(penEjes, centroX, 0, centroX, previewCanvas->Height);
            delete penEjes;

            std::vector<Punto2D> pts = CalcularGeometriaFrame(frameActualIndex);
            if (pts.empty()) return;

            Pen^ penFigura = gcnew Pen(Color::FromArgb(0, 240, 255), 2.5f);
            for (size_t i = 0; i < pts.size() - 1; ++i) {
                int x1 = centroX + (int)std::round(pts[i].x);
                int y1 = centroY - (int)std::round(pts[i].y);
                int x2 = centroX + (int)std::round(pts[i + 1].x);
                int y2 = centroY - (int)std::round(pts[i + 1].y);
                g->DrawLine(penFigura, x1, y1, x2, y2);
            }
            delete penFigura;
        }
    };
}