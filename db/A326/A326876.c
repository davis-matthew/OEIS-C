/*
 * A326876  BII-numbers of finite topologies without their empty set.
 * https://oeis.org/A326876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) consists of all natural numbers n (including 0) for which
   the set system obtained by the BII construction is a finite topology
   (excluding the empty open set).  We enumerate numbers in increasing order
   and return the n‑th one found.  The test constructs the family
   of sets from a candidate BII–number and checks the axioms of a finite
   topology: presence of the universal set and closure under union and
   intersection.  Complexity is O(k²) per candidate, where k is the number
   of non‑empty open sets; this is sufficient for the required term range. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* maximal number of open sets that can occur for a 64‑bit candidate */
#define MAXSETS  64

/* linear search for a bitmask in an array of masks */
static inline bool contains(const uint64_t *arr, size_t cnt, uint64_t val)
{
    for (size_t i = 0; i < cnt; ++i)
        if (arr[i] == val) return true;
    return false;
}

/* Test whether the BII-number n represents a finite topology
   (the empty open set is omitted).  The construction is:
        1. For each 1‑bit of n (in reversed binary order) take its
           position pos (1‑based).
        2. For that pos, take the positions of 1‑bits in the binary
           representation of pos; these positions form a set represented
           by a bitmask.
   The set of all such masks must contain the universal set and
   be closed under union and intersection. */
static inline bool is_topology(uint64_t n)
{
    uint64_t masks[MAXSETS];
    size_t   sz   = 0;
    uint64_t U    = 0;            /* universal set mask */
    uint64_t bits = n;
    uint64_t bitpos = 1;          /* 1‑based bit position */

    while (bits) {
        if (bits & 1ULL) {
            uint64_t pos  = bitpos;          /* current index */
            uint64_t sub  = pos;
            uint64_t mask = 0;
            uint64_t subpos = 1;             /* 1‑based sub‑position */

            while (sub) {
                if (sub & 1ULL)
                    mask |= 1ULL << (subpos - 1);
                sub >>= 1;
                ++subpos;
            }

            if (mask) {                      /* non‑empty set */
                masks[sz++]   = mask;
                U              |= mask;       /* accumulate universal set */
            }
        }
        bits >>= 1;
        ++bitpos;
    }

    /* The trivial topology on the empty set: no non‑empty open sets. */
    if (sz == 0)
        return true;

    /* The universal set must be present among the masks. */
    bool hasU = false;
    for (size_t i = 0; i < sz; ++i) {
        if (masks[i] == U) {
            hasU = true;
            break;
        }
    }
    if (!hasU)
        return false;

    /* Check closure under union and intersection. */
    for (size_t i = 0; i < sz; ++i) {
        for (size_t j = i; j < sz; ++j) {
            uint64_t uni = masks[i] | masks[j];
            if (uni != 0 && uni != U && !contains(masks, sz, uni))
                return false;

            uint64_t inter = masks[i] & masks[j];
            if (inter != 0 && inter != U && !contains(masks, sz, inter))
                return false;
        }
    }

    return true;
}

/* Return the n‑th BII‑number of a finite topology (index starts at 1). */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, but safe */
        return (seq_t)0;

    idx_t found   = 0;
    uint64_t cand = 0;

    while (found < n) {
        if (is_topology(cand)) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
        ++cand;
        /* Under the problem constraints the loop will terminate before
           uint64_t overflows, so no explicit wrap‑around handling is
           required. */
    }

    /* unreachable */
    return (seq_t)0;
}
