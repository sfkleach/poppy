#include "cell.hpp"
#include "gc.hpp"

namespace poppy {

GarbageCollector::GarbageCollector(Engine & engine) {

}

GarbageCollector::~GarbageCollector() {

}

void GarbageCollector::visitCell(Cell & cell) {

}

void GarbageCollector::visitObject(Cell * object) {

}

void GarbageCollector::enqueueObject(Cell * object) {

}

void GarbageCollector::scanObject(Cell * object) {

}

} // namespace poppy
