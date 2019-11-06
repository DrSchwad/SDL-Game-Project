class Color {
	public:
	Color();
	Color(int r, int g, int b, int a = 0xFF);
	int r, g, b, a;
};

Color::Color() {
	r = g = b = 0, a = 0xFF;
}

Color::Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}