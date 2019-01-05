#include <QApplication>
#include <QPlainTextEdit>
#include <QPalette>
#include <QFontDatabase>

extern "C" {
#include "../highlighter.h"
}

static const QColor colormap[] = {
	[HighlightSection] = QColor("#ababab"),
	[HighlightKeytype] = QColor("#70c0b1"),
	[HighlightKey] = QColor("#7aa6da"),
	[HighlightIP] = QColor("#b9ca4a"),
	[HighlightCidr] = QColor("#e78c45"),
	[HighlightHost] = QColor("#b9ca4a"),
	[HighlightPort] = QColor("#e78c45"),
	[HighlightMTU] = QColor("#c397d8"),
	[HighlightKeepalive] = QColor("#c397d8"),
	[HighlightComment] = QColor("#969896"),
	[HighlightDelimiter] = QColor("#7aa6da"),
#ifndef MOBILE_WGQUICK_SUBSET
	[HighlightCmd] = QColor("#969896"),
	[HighlightTable] = QColor("#c397d8"),
	[HighlightFwMark] = QColor("#c397d8"),
	[HighlightSaveConfig] = QColor("#c397d8"),
#endif
	[HighlightError] = QColor("#d54e53")
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QPlainTextEdit e;
	QPalette p(e.palette());
	p.setColor(QPalette::Base, QColor("#010101"));
	p.setColor(QPalette::Text, QColor("#eaeaea"));
	e.setPalette(p);
	QFont f(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	f.setPointSize(16);
	e.setFont(f);
	e.setMinimumSize(400, 500);
	bool guard = false;
	QObject::connect(&e, &QPlainTextEdit::textChanged, [&]() {
		if (guard)
			return;
		struct highlight_span *spans = highlight_config(e.toPlainText().toLatin1().data());
		if (!spans)
			return;
		QTextCursor cursor(e.document());
		QTextCharFormat format;
		cursor.beginEditBlock();
		cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
		format.setForeground(p.color(QPalette::Text));
		cursor.mergeCharFormat(format);
		for (struct highlight_span *span = spans; span->type != HighlightEnd; ++span) {
			cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
			cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, span->start);
			cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, span->len);
			format.setForeground(colormap[span->type]);
			cursor.mergeCharFormat(format);
		}
		free(spans);
		guard = true;
		cursor.endEditBlock();
		guard = false;
	});

	e.show();
	return a.exec();
}
