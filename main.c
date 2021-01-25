
#include <lvmx.h>

int textbuff[16];
int resultbuff[16];
int buffitr = 0;
float accum;
int headerText;
int opcache = 0;

// 現在入力中の値は、文字列の末尾に押されたキーの数字(+ピリオド)に対応する文字を
// 追加することで実装している。(計算するときにstringからfloatに変換する)
void checkDigitButton(int button, int digit) {
	// UIXButtonテンプレートは、ボタンが押されると"pressed" Dynamic Variableが1になるように
	// なっている。なので、その値をチェックして、もし1になっているなら押されたときの処理を行い、
	// 数値を0に書き戻してあげることでボタンの挙動を再現できる。
	if (getDVInt(button, "pressed") != 0) {
		setDVInt(button, "pressed", 0);

		// ここで文字列の末尾に文字を追加している。Cでの文字列は「0終端文字列」なので、
		// 文字の終わりに0を書きこんでおくことで長さを変えることができる。
		textbuff[buffitr++] = digit;
		textbuff[buffitr] = 0;

		// 現在の入力途中の文字を表示
		setDVString(headerText, "content", textbuff);
	}
}

void checkOpButton(int button, int op) {
	if (getDVInt(button, "pressed") != 0) {
		setDVInt(button, "pressed", 0);

		// opcache: 前回入力されていた演算
		// これによって計算内容を変える
		switch(opcache) {
			case '+':
				accum += stof(textbuff);
				break;
			case '-':
				accum -= stof(textbuff);
				break;
			case '*':
				accum *= stof(textbuff);
				break;
			case '/':
				accum /= stof(textbuff);
				break;
			case '=':
				break;
			default:
				accum = stof(textbuff);
				break;
		}
		opcache = op;
		ftos(accum, resultbuff); // accumのfloatを文字列に変換してresultbuffに書き込む
		setDVString(headerText, "content", resultbuff); // 計算結果の表示
		buffitr = 0;
	}
}

int main() {

	// ヘッダー(計算結果の表示部分)を作る
	int headerRect = createSlotFromTemplate("UIXempty"); // UIXemptyはRectの設定ができるだけのUIXテンプレート
														 // (AM: Anchor Max, Am: Anchor min, OM: Offset Max, Om: offset min) + xy
														 // 全部float
	setDVFloat(headerRect, "Amy", 0.8f);
	setSlotParent(headerRect, UIXROOT_SLOT); // setSlotParentするまではslotはtmp領域に保存されている

	headerText = createSlotFromTemplate("UIXtext"); // UIXtextはtext
													// content: string, size: float, R: float, G: float, B: float
	setDVString(headerText, "content", "0");
	setDVFloat(headerText, "size", 48.0f);
	setDVFloat(headerText, "R", 0.9f);
	setDVFloat(headerText, "G", 0.66f);
	setDVFloat(headerText, "B", 0.23f);
	setSlotParent(headerText, headerRect);

	// ボタンが入る部分
	int digitRect = createSlotFromTemplate("UIXempty");
	setDVFloat(digitRect, "AMy", 0.8f);
	setSlotParent(digitRect, UIXROOT_SLOT);

	// ボタンはVertical LayoutとHorizontal Layoutを使って並べる
	int vlayout = createSlotFromTemplate("UIXlayoutV"); // Virtical Layout 設定項目はなし
	setSlotParent(vlayout, digitRect);

	int hlayout0 = createSlotFromTemplate("UIXlayoutH"); // Horizontal Layout 設定項目はなし
	setSlotParent(hlayout0, vlayout);

	int hlayout1 = dupSlot(hlayout0); // hlayout0はvlayoutにペアレント済なので、またsetParentしなくても
	int hlayout2 = dupSlot(hlayout0); // vlayoutにペアレントされている
	int hlayout3 = dupSlot(hlayout0);
	int hlayout4 = dupSlot(hlayout0);

	// 数値ボタンのテンプレートを作る
	// これをコピーして使うことで、色などを再設定する手間を省ける
	int digitButtonTemp = createSlotFromTemplate("UIXbutton"); // UIXButton パラメータ:  pressed: int
	int buttonText = createSlotFromTemplate("UIXtext");
	setDVFloat(buttonText, "R", 0.87f);
	setDVFloat(buttonText, "G", 0.87f);
	setDVFloat(buttonText, "B", 0.87f);
	setSlotParent(buttonText, digitButtonTemp);

	// 演算ボタンのテンプレートを作る
	int opButtonTemp = createSlotFromTemplate("UIXbutton");
	int opButtonImg  = createSlotFromTemplate("UIXimage"); // UIXimage パラメータ: R: float G: float B: float
	setDVFloat(opButtonImg, "R", 0.9f);
	setDVFloat(opButtonImg, "G", 0.66f);
	setDVFloat(opButtonImg, "B", 0.23f);
	setSlotParent(opButtonImg, opButtonTemp);
	int opButtonText = createSlotFromTemplate("UIXtext");
	setDVFloat(opButtonText, "R", 0.2f);
	setDVFloat(opButtonText, "G", 0.2f);
	setDVFloat(opButtonText, "B", 0.2f);
	setSlotParent(opButtonText, opButtonImg);

	// 1行目のボタン群
	setDVString(buttonText, "content", "");
	int dummy = dupSlot(digitButtonTemp);
	setSlotParent(dummy, hlayout0);
	setDVString(buttonText, "content", "CE");
	int buttonCE = dupSlot(digitButtonTemp);
	setSlotParent(buttonCE, hlayout0);
	setDVString(buttonText, "content", "C");
	int buttonC = dupSlot(digitButtonTemp);
	setSlotParent(buttonC, hlayout0);
	setDVString(opButtonText, "content", "÷");
	int buttonDiv = dupSlot(opButtonTemp);
	setSlotParent(buttonDiv, hlayout0);

	// 2行目のボタン群
	setDVString(buttonText, "content", "7");
	int button7 = dupSlot(digitButtonTemp);
	setSlotParent(button7, hlayout1);
	setDVString(buttonText, "content", "8");
	int button8 = dupSlot(digitButtonTemp);
	setSlotParent(button8, hlayout1);
	setDVString(buttonText, "content", "9");
	int button9 = dupSlot(digitButtonTemp);
	setSlotParent(button9, hlayout1);
	setDVString(opButtonText, "content", "x");
	int buttonMul = dupSlot(opButtonTemp);
	setSlotParent(buttonMul, hlayout1);

	// 3行目のボタン群
	setDVString(buttonText, "content", "4");
	int button4 = dupSlot(digitButtonTemp);
	setSlotParent(button4, hlayout2);
	setDVString(buttonText, "content", "5");
	int button5 = dupSlot(digitButtonTemp);
	setSlotParent(button5, hlayout2);
	setDVString(buttonText, "content", "6");
	int button6 = dupSlot(digitButtonTemp);
	setSlotParent(button6, hlayout2);
	setDVString(opButtonText, "content", "-");
	int buttonSub = dupSlot(opButtonTemp);
	setSlotParent(buttonSub, hlayout2);

	// 4行目のボタン群
	setDVString(buttonText, "content", "1");
	int button1 = dupSlot(digitButtonTemp);
	setSlotParent(button1, hlayout3);
	setDVString(buttonText, "content", "2");
	int button2 = dupSlot(digitButtonTemp);
	setSlotParent(button2, hlayout3);
	setDVString(buttonText, "content", "3");
	int button3 = dupSlot(digitButtonTemp);
	setSlotParent(button3, hlayout3);
	setDVString(opButtonText, "content", "+");
	int buttonAdd = dupSlot(opButtonTemp);
	setSlotParent(buttonAdd, hlayout3);

	// 5行目のボタン群
	setDVString(buttonText, "content", "");
	dummy = dupSlot(digitButtonTemp);
	setSlotParent(dummy, hlayout4);
	setDVString(buttonText, "content", "0");
	int button0 = dupSlot(digitButtonTemp);
	setSlotParent(button0, hlayout4);
	setDVString(buttonText, "content", ".");
	int buttonDot = dupSlot(digitButtonTemp);
	setSlotParent(buttonDot, hlayout4);
	setDVString(opButtonText, "content", "=");
	int buttonRes = dupSlot(opButtonTemp);
	setSlotParent(buttonRes, hlayout4);

	// UI要素を作り終わったので、動作クロックを上げる
	//     (UI要素の作成を高クロックで行うと、Slot生成の遅延やDynamicVariableの
	//      リンクの遅延によって失敗することがあるため)
	setDVInt(DEVICEROOT_SLOT, "OVERCLOCK", 10);

	// 無限ループにしてアプリが終了しないようにする
	while (1) {

		// 電卓アプリの動作本体
		// ボタンの入力をチェックして、それに応じて処理をする。
		checkDigitButton(button0, '0');
		checkDigitButton(button1, '1');
		checkDigitButton(button2, '2');
		checkDigitButton(button3, '3');
		checkDigitButton(button4, '4');
		checkDigitButton(button5, '5');
		checkDigitButton(button6, '6');
		checkDigitButton(button7, '7');
		checkDigitButton(button8, '8');
		checkDigitButton(button9, '9');
		checkDigitButton(buttonDot, '.');

		checkOpButton(buttonAdd, '+');
		checkOpButton(buttonSub, '-');
		checkOpButton(buttonMul, '*');
		checkOpButton(buttonDiv, '/');
		checkOpButton(buttonRes, '=');

		if (getDVInt(buttonC, "pressed") != 0) {
			setDVInt(buttonC, "pressed", 0);
			opcache = 0;
			accum = 0;
			buffitr = 0;
			textbuff[0] = 0;
			setDVString(headerText, "content", "0");
		}

		if (getDVInt(buttonCE, "pressed") != 0) {
			setDVInt(buttonCE, "pressed", 0);
			buffitr = 0;
			textbuff[0] = 0;
			setDVString(headerText, "content", "0");
		}

	}

	return 0;
}
