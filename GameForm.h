#pragma once

#include <time.h>
#include <stdlib.h>

namespace SnakeGame {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Collections::Generic;

    /// <summary>
    /// Сводка для GameForm
    /// </summary>
    public ref class GameForm : public System::Windows::Forms::Form
    {
    public:
        GameForm(void)
        {
            InitializeComponent();
            //
            //TODO: добавьте код конструктора
            //
        }

    protected:
        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        ~GameForm()
        {
            if (components)
            {
                delete components;
            }
        }

        virtual void OnPaint(System::Object^ obj, PaintEventArgs^ e) override
        {
            Graphics^ g = e->Graphics;

            Brush^ fruitBrush = gcnew SolidBrush(Color::Red);
            g->FillEllipse(fruitBrush, fruitPosition.X, fruitPosition.Y, blockSize, blockSize);
            Brush^ snakeBrush = gcnew SolidBrush(Color::Green);
            for each (Point el in snake)
            {
                g->FillRectangle(snakeBrush, el.X, el.Y, blockSize, blockSize);
            }
        }

    private:
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        System::ComponentModel::Container^ components;
        List<Point>^ snake;
        Point fruitPosition;
        const int blockSize = 20;

        int fruitCount = 0;
        Label^ scoreLabel; //создание счётчика фруктов

        Timer^ timer;
        int moveX = 1, moveY = 0;


#pragma region Windows Form Designer generated code
        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        void InitializeComponent(void)
        {
            this->SuspendLayout();
            // 
            // GameForm
            // 
            this->ClientSize = System::Drawing::Size(400, 400);
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(GameForm::typeid));
            this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
            this->Text = L"Snake Game";
            //this->Text = L"Snake";
            this->Load += gcnew System::EventHandler(this, &GameForm::GameForm_Load);
            this->BackColor = Color::Black;
            this->ResumeLayout(false);

            scoreLabel = gcnew Label();
            scoreLabel->ForeColor = Color::White;
            scoreLabel->BackColor = Color::Transparent;
            scoreLabel->Text = "Score: 0";
            scoreLabel->Location = Point(10, 10);
            this->Controls->Add(scoreLabel);

            snake = gcnew List<Point>();
            snake->Add(Point(100, 100));
            srand(time(NULL));
            PlaceFruit();

            timer = gcnew Timer();
            timer->Interval = 100;
            timer->Tick += gcnew EventHandler(this, &GameForm::OnTimerTick);
            timer->Start();

            this->Paint += gcnew PaintEventHandler(this, &GameForm::OnPaint);
            this->KeyDown += gcnew KeyEventHandler(this, &GameForm::OnKeyDown);

        }
#pragma endregion

        void PlaceFruit() // рандомно рисует фрукт на поле
        {
            int maxX = this->ClientSize.Width / blockSize;
            int maxY = this->ClientSize.Height / blockSize;

            do
            {
                fruitPosition = Point(rand() % maxX * blockSize, rand() % maxY * blockSize);
            } while (snake->Contains(fruitPosition));


        }

        void OnTimerTick(Object^ obj, EventArgs^ e)
        {
            MoveSnake();

            if (snake[0].X < 0 || snake[0].Y < 0 || snake[0].X >= this->ClientSize.Width || snake[0].Y >= this->ClientSize.Width)// Если выходим за границы, тотпроигрываем.
            {
                timer->Stop();
                MessageBox::Show("GAME OVER :(");
                return;
            }

            if (snake->Count >= 4)
            {
                for (int i = 1; i < snake->Count; i++)
                {
                    if (snake[0] == snake[i])
                    {
                        timer->Stop();
                        MessageBox::Show("GAME OVER :(");
                        return;
                    }
                }
            }

            if (snake[0] == fruitPosition) // Если координаты совпадают то фрукту присваиваются новые координаты
            {
                fruitCount++;
                scoreLabel->Text = "Score: " + fruitCount.ToString();
                GrowthSnake();
                PlaceFruit();
            }

            this->Invalidate();

        }

        void MoveSnake() //Передвигает змейку
        {
            Point newHead = snake[0];

            newHead.X += moveX * blockSize;
            newHead.Y += moveY * blockSize;
            snake->Insert(0, newHead);
            snake->RemoveAt(snake->Count - 1);
        }

        void GrowthSnake()
        {
            fruitCount++;
            scoreLabel->Text = "Score: " + fruitCount.ToString();//Устанавливаем новое значение текста
            Point newHead = snake[0];

            newHead.X += moveX * blockSize;
            newHead.Y += moveY * blockSize;
            snake->Insert(0, newHead);
        }
        void OnKeyDown(Object^ obj, KeyEventArgs^ e)
        {
            switch (e->KeyCode)
            {
            case Keys::Up:
                moveX = 0;
                moveY = -1;
                break;
            case Keys::Down:
                moveX = 0;
                moveY = 1;
                break;
            case Keys::Left:
                moveX = -1;
                moveY = 0;
                break;
            case Keys::Right:
                moveX = 1;
                moveY = 0;
                break;
            }
        }

    private: System::Void GameForm_Load(System::Object^ sender, System::EventArgs^ e) {
    }
    };
}