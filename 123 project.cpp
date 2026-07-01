// ============================================================
// LUXURY FOOD DELIVERY SYSTEM (FINAL VERSION)
// C++98 WIN32 API
// ============================================================

#include <windows.h>
#include <string>
#include <queue>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

// ================= BUTTON IDS =================
#define BTN_LOGIN      100
#define BTN_PIZZA      1
#define BTN_BURGER     2
#define BTN_PASTA      3
#define BTN_DRINK      4
#define BTN_DESSERT    5
#define BTN_HISTORY    6
#define BTN_BILL       7
#define BTN_TRACK      8
#define BTN_EXIT       9
#define BTN_ADDQTY     10
#define BTN_RIDER_YES  11
#define BTN_RIDER_NO   12

// ================= GLOBALS =================
HWND hUser, hPass, hAddress, hQty, hRiderInfo;

queue<string> orderQueue;

string currentItem;
int currentPrice = 0;
int totalBill = 0;
string toAddress;

// ================= RIDERS =================
vector<string> riders;

void initRiders()
{
    riders.push_back("Ali (Bike)");
    riders.push_back("Hassan (Car)");
    riders.push_back("Ahmed (Bike)");
    riders.push_back("Usman (Scooter)");
}

// ================= USER RIDER CHOICE =================
bool riderRequired = false;

// ================= TIME =================
string getDateTime()
{
    time_t now = time(0);
    tm *t = localtime(&now);

    stringstream ss;
    ss << t->tm_mday << "/"
       << 1 + t->tm_mon << "/"
       << 1900 + t->tm_year << " "
       << t->tm_hour << ":" << t->tm_min;

    return ss.str();
}

string getDay()
{
    time_t now = time(0);
    tm *t = localtime(&now);

    string days[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    return days[t->tm_wday];
}

string toStr(int x)
{
    stringstream ss;
    ss << x;
    return ss.str();
}

// ================= QUANTITY WINDOW =================
LRESULT CALLBACK QtyProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{
    switch(msg)
    {
    case WM_CREATE:
        CreateWindow("STATIC","Enter Quantity",
            WS_VISIBLE|WS_CHILD,
            60,20,180,20,hwnd,NULL,NULL,NULL);

        hQty = CreateWindow("EDIT","1",
            WS_VISIBLE|WS_CHILD|WS_BORDER,
            60,50,120,25,hwnd,NULL,NULL,NULL);

        CreateWindow("BUTTON","ADD TO CART",
            WS_VISIBLE|WS_CHILD,
            60,90,140,30,hwnd,(HMENU)BTN_ADDQTY,NULL,NULL);
        break;

    case WM_COMMAND:
        if(LOWORD(wp)==BTN_ADDQTY)
        {
            char q[10];
            GetWindowText(hQty,q,10);

            int qty = atoi(q);
            if(qty <= 0) qty = 1;

            string rider;

            // ===== RIDER DECISION =====
            if(riderRequired && !riders.empty())
                rider = riders[rand() % riders.size()];
            else
                rider = "No Rider Assigned";

            string order =
                "ITEM: " + currentItem +
                " | QTY: " + toStr(qty) +
                " | TOTAL: Rs " + toStr(currentPrice * qty) +
                " | RIDER: " + rider +
                " | DAY: " + getDay() +
                " | DATE/TIME: " + getDateTime();

            orderQueue.push(order);
            totalBill += currentPrice * qty;

            MessageBox(hwnd,"ORDER PLACED SUCCESSFULLY","SUCCESS",MB_OK);
            DestroyWindow(hwnd);
        }
        break;
    }

    return DefWindowProc(hwnd,msg,wp,lp);
}

// ================= OPEN QTY =================
void openQty(HWND parent,string item,int price)
{
    currentItem = item;
    currentPrice = price;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = QtyProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "QtyClass";
    wc.hbrBackground = CreateSolidBrush(RGB(35,35,70));

    RegisterClass(&wc);

    CreateWindow("QtyClass","Quantity Panel",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        600,200,260,200,
        NULL,NULL,NULL,NULL);
}

// ================= HISTORY =================
void showHistory(HWND hwnd)
{
    if(orderQueue.empty())
    {
        MessageBox(hwnd,"No Orders Yet","INFO",MB_OK);
        return;
    }

    string h;
    queue<string> temp = orderQueue;

    while(!temp.empty())
    {
        h += temp.front() + "\n\n";
        temp.pop();
    }

    MessageBox(hwnd,h.c_str(),"ORDER HISTORY",MB_OK);
}

// ================= FOOD WINDOW =================
LRESULT CALLBACK FoodProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{
    switch(msg)
    {
    case WM_CREATE:

        hRiderInfo = CreateWindow("STATIC","RIDER OPTION: NOT SELECTED",
            WS_VISIBLE|WS_CHILD,
            40,10,300,20,hwnd,NULL,NULL,NULL);

        CreateWindow("BUTTON","Rider YES",
            WS_VISIBLE|WS_CHILD,
            80,40,120,25,hwnd,(HMENU)BTN_RIDER_YES,NULL,NULL);

        CreateWindow("BUTTON","Rider NO",
            WS_VISIBLE|WS_CHILD,
            210,40,120,25,hwnd,(HMENU)BTN_RIDER_NO,NULL,NULL);

        CreateWindow("BUTTON","Pizza - 1200",
            WS_VISIBLE|WS_CHILD,
            80,80,200,30,hwnd,(HMENU)BTN_PIZZA,NULL,NULL);

        CreateWindow("BUTTON","Burger - 500",
            WS_VISIBLE|WS_CHILD,
            80,120,200,30,hwnd,(HMENU)BTN_BURGER,NULL,NULL);

        CreateWindow("BUTTON","Pasta - 800",
            WS_VISIBLE|WS_CHILD,
            80,160,200,30,hwnd,(HMENU)BTN_PASTA,NULL,NULL);

        CreateWindow("BUTTON","Drink - 200",
            WS_VISIBLE|WS_CHILD,
            80,200,200,30,hwnd,(HMENU)BTN_DRINK,NULL,NULL);

        CreateWindow("BUTTON","Dessert - 400",
            WS_VISIBLE|WS_CHILD,
            80,240,200,30,hwnd,(HMENU)BTN_DESSERT,NULL,NULL);

        CreateWindow("BUTTON","History",
            WS_VISIBLE|WS_CHILD,
            80,290,200,30,hwnd,(HMENU)BTN_HISTORY,NULL,NULL);

        CreateWindow("BUTTON","Bill",
            WS_VISIBLE|WS_CHILD,
            80,330,200,30,hwnd,(HMENU)BTN_BILL,NULL,NULL);

        CreateWindow("BUTTON","Track",
            WS_VISIBLE|WS_CHILD,
            80,370,200,30,hwnd,(HMENU)BTN_TRACK,NULL,NULL);

        CreateWindow("BUTTON","Exit",
            WS_VISIBLE|WS_CHILD,
            80,410,200,30,hwnd,(HMENU)BTN_EXIT,NULL,NULL);
        break;

    case WM_COMMAND:
        switch(LOWORD(wp))
        {
        case BTN_RIDER_YES:
            riderRequired = true;
            SetWindowText(hRiderInfo,"RIDER OPTION: YES");
            break;

        case BTN_RIDER_NO:
            riderRequired = false;
            SetWindowText(hRiderInfo,"RIDER OPTION: NO");
            break;

        case BTN_PIZZA:  openQty(hwnd,"Pizza",1200); break;
        case BTN_BURGER: openQty(hwnd,"Burger",500); break;
        case BTN_PASTA:  openQty(hwnd,"Pasta",800); break;
        case BTN_DRINK:  openQty(hwnd,"Drink",200); break;
        case BTN_DESSERT:openQty(hwnd,"Dessert",400); break;

        case BTN_HISTORY:
            showHistory(hwnd);
            break;

        case BTN_BILL:
            MessageBox(hwnd,
                ("TOTAL BILL: Rs " + toStr(totalBill)).c_str(),
                "BILL",MB_OK);
            break;

        case BTN_TRACK:
            MessageBox(hwnd,
                ("Order is on the way!\nTime: " + getDateTime()).c_str(),
                "TRACK",MB_OK);
            break;

        case BTN_EXIT:
            PostQuitMessage(0);
            break;
        }
        break;
    }

    return DefWindowProc(hwnd,msg,wp,lp);
}

// ================= LOGIN =================
LRESULT CALLBACK LoginProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{
    switch(msg)
    {
    case WM_CREATE:

        CreateWindow("STATIC","USERNAME",
            WS_VISIBLE|WS_CHILD,
            50,20,200,20,hwnd,NULL,NULL,NULL);

        hUser = CreateWindow("EDIT","",
            WS_VISIBLE|WS_CHILD|WS_BORDER,
            50,50,220,25,hwnd,NULL,NULL,NULL);

        CreateWindow("STATIC","PASSWORD",
            WS_VISIBLE|WS_CHILD,
            50,80,200,20,hwnd,NULL,NULL,NULL);

        hPass = CreateWindow("EDIT","",
            WS_VISIBLE|WS_CHILD|WS_BORDER|ES_PASSWORD,
            50,110,220,25,hwnd,NULL,NULL,NULL);

        CreateWindow("STATIC","ADDRESS",
            WS_VISIBLE|WS_CHILD,
            50,140,200,20,hwnd,NULL,NULL,NULL);

        hAddress = CreateWindow("EDIT","",
            WS_VISIBLE|WS_CHILD|WS_BORDER,
            50,170,220,25,hwnd,NULL,NULL,NULL);

        CreateWindow("BUTTON","LOGIN",
            WS_VISIBLE|WS_CHILD,
            90,210,120,30,hwnd,(HMENU)BTN_LOGIN,NULL,NULL);
        break;

    case WM_COMMAND:
        if(LOWORD(wp)==BTN_LOGIN)
        {
            char u[50],p[50],a[100];

            GetWindowText(hUser,u,50);
            GetWindowText(hPass,p,50);
            GetWindowText(hAddress,a,100);

            string user = u;
            string pass = p;
            toAddress = a;

            if(user=="noorseeham" && pass=="1234")
            {
                MessageBox(hwnd,"LOGIN SUCCESS","OK",MB_OK);

                CreateWindow("FoodClass","Menu",
                    WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                    400,100,360,520,
                    NULL,NULL,GetModuleHandle(NULL),NULL);

                DestroyWindow(hwnd);
            }
            else
            {
                MessageBox(hwnd,"INVALID LOGIN","ERROR",MB_OK);
            }
        }
        break;
    }

    return DefWindowProc(hwnd,msg,wp,lp);
}

// ================= MAIN =================
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int)
{
    srand(time(0));
    initRiders();

    WNDCLASS wc1 = {0};
    wc1.lpfnWndProc = LoginProc;
    wc1.hInstance = hInst;
    wc1.lpszClassName = "LoginClass";
    wc1.hbrBackground = CreateSolidBrush(RGB(10,10,25));
    RegisterClass(&wc1);

    WNDCLASS wc2 = {0};
    wc2.lpfnWndProc = FoodProc;
    wc2.hInstance = hInst;
    wc2.lpszClassName = "FoodClass";
    wc2.hbrBackground = CreateSolidBrush(RGB(18,18,35));
    RegisterClass(&wc2);

    CreateWindow("LoginClass","Login System",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        500,150,350,300,
        NULL,NULL,hInst,NULL);

    MSG msg;
    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
