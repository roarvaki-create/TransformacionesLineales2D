#pragma once

namespace TransformacionesLineales2D {

    using namespace System;
    using namespace System::Windows::Forms;

    public ref class DoubleBufferedPanel : public System::Windows::Forms::Panel {
    public:
        DoubleBufferedPanel() {
            this->DoubleBuffered = true;
            this->SetStyle(static_cast<ControlStyles>(
                ControlStyles::AllPaintingInWmPaint |
                ControlStyles::UserPaint |
                ControlStyles::OptimizedDoubleBuffer), true);
            this->UpdateStyles();
        }
    };
}
