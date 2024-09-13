cell.o: cell.cpp cell.hpp layout.hpp
codeplanter.o: codeplanter.cpp itemizer.hpp item.hpp itemattrs.hpp \
 itemrole.hpp cell.hpp layout.hpp heap.hpp mishap.hpp xroots.hpp \
 engine.hpp iscanner.hpp codeplanter.hpp
engine.o: engine.cpp itemizer.hpp item.hpp itemattrs.hpp itemrole.hpp \
 cell.hpp layout.hpp heap.hpp mishap.hpp xroots.hpp engine.hpp \
 iscanner.hpp gc.hpp
gc.o: gc.cpp cell.hpp layout.hpp gc.hpp engine.hpp itemizer.hpp item.hpp \
 itemattrs.hpp itemrole.hpp heap.hpp mishap.hpp xroots.hpp iscanner.hpp
heap.o: heap.cpp heap.hpp cell.hpp layout.hpp mishap.hpp
item.o: item.cpp item.hpp itemattrs.hpp itemrole.hpp
itemattrs.o: itemattrs.cpp itemattrs.hpp
itemizer.o: itemizer.cpp itemizer.hpp item.hpp itemattrs.hpp itemrole.hpp
itemrole.o: itemrole.cpp itemrole.hpp itemattrs.hpp
mishap.o: mishap.cpp mishap.hpp
poppy.o: poppy.cpp itemizer.hpp item.hpp itemattrs.hpp itemrole.hpp \
 cell.hpp layout.hpp heap.hpp mishap.hpp xroots.hpp engine.hpp \
 iscanner.hpp codeplanter.hpp
xroots.o: xroots.cpp xroots.hpp cell.hpp layout.hpp
