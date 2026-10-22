/*
 * A308619  Negative van Eck's sequence: For n >= 1, if there exists an m < n
 *          such that a(m) = a(n), take the largest such m and set a(n+1) =
 *          n-m, otherwise a(n+1) = -a(n). Start with a(1)=0.
 * https://oeis.org/A308619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute OEIS A308619: Negative van Eck's sequence
   Recurrence:  a(1)=0.
                For n>=1, let m be the largest index < n with a(m)=a(n),
                if it exists set a(n+1)=n-m,
                otherwise set a(n+1) = -a(n).
   The function caches computed values for fast service of any order of calls.
*/

#include <stdlib.h>
#include <stddef.h>

typedef struct { seq_t val; idx_t last; } map_entry_t;

static seq_t *cache = NULL;          /* cache[1..maxComputed] holds a(i)  */
static size_t cacheCap = 0;          /* current allocated capacity */
static idx_t  maxComputed = 0;       /* largest n already computed */

static map_entry_t *map = NULL;      /* mapping from value to last index */
static size_t mapSize = 0;           /* number of entries */
static size_t mapCap = 0;            /* allocated capacity for map */

static inline void ensure_cache(idx_t need)
{
    if ((size_t)(need + 1) > cacheCap) {
        size_t newCap = cacheCap ? cacheCap * 2 : 16;
        if (newCap < (size_t)(need + 1))
            newCap = (size_t)(need + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failed */
        cache = tmp;
        cacheCap = newCap;
    }
}

static inline void ensure_map(size_t need)
{
    if (need > mapCap) {
        size_t newCap = mapCap ? mapCap * 2 : 16;
        if (newCap < need)
            newCap = need;
        map_entry_t *tmp = (map_entry_t *)realloc(map, newCap * sizeof(map_entry_t));
        if (!tmp) exit(1);
        map = tmp;
        mapCap = newCap;
    }
}

seq_t a(idx_t n)
{
    /* initialise structures on first call */
    if (!cache) {
        cacheCap = 16;
        cache = (seq_t *)calloc(cacheCap, sizeof(seq_t));
        if (!cache) exit(1);
        cache[1] = 0;
        maxComputed = 1;
    }
    if (!map) {
        mapCap = 16;
        map = (map_entry_t *)malloc(mapCap * sizeof(map_entry_t));
        if (!map) exit(1);
        mapSize = 0;
    }

    /* extend cache up to n, if necessary */
    while (maxComputed < n) {
        idx_t i = maxComputed;             /* current index */
        seq_t val = cache[i];

        /* find previous occurrence of val */
        idx_t previous = 0;
        size_t foundIdx = (size_t)(-1);
        for (size_t j = 0; j < mapSize; ++j) {
            if (map[j].val == val) {
                previous = map[j].last;
                foundIdx = j;
                break;
            }
        }

        seq_t next;
        if (previous != 0) {
            next = (seq_t)(i - previous);
        } else {
            next = -val;
        }

        /* store next term */
        idx_t nextIdx = i + 1;
        ensure_cache(nextIdx);
        cache[nextIdx] = next;

        /* update mapping for current value (val) */
        if (previous == 0) {
            /* first time we see this value */
            ensure_map(mapSize + 1);
            map[mapSize].val = val;
            map[mapSize].last = i;
            ++mapSize;
        } else {
            /* update last occurrence */
            map[foundIdx].last = i;
        }

        ++maxComputed;
    }

    return cache[n];
}
