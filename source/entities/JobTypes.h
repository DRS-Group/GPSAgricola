// JobTypes.h
#ifndef JOBTYPES_H
#define JOBTYPES_H

#include <cstdint>
enum class JobType : uint8_t {
    Spray = 0,
    Planter = 1,
    SpotSpray = 2
};

#endif // JOBTYPES_H
