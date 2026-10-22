/*
 * A321778  Number of compositions of n into parts with distinct
 *          multiplicities and with exactly eight parts.
 * https://oeis.org/A321778
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into exactly eight parts *
 *            with distinct multiplicities of part sizes
 * Range:     u8 n <= 12, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation enumerates the partitions of 8 into distinct
 * multiplicity multisets.  For each such multiset it solves a linear
 * Diophantine equation for the distinct part sizes and adds the number
 * of ordered compositions (8! divided by the factorials of the
 * multiplicities).  This runs in O(n²) for the 3‑part partitions and
 * is well below the required time for n up to ~10000. */

#include <stdint.h>

static const seq_t fact[9] = {1u,1u,2u,6u,24u,120u,720u,5040u,40320u};

typedef struct {
    uint8_t k;      /* number of distinct multiplicities in this partition */
    uint8_t m[3];   /* the multiplicities (padded with 0 if k<3) */
    seq_t ratio;    /* 8! divided by the product of factorials of the multiplicities */
} Part;

static const Part parts[] = {
    {1, {8,0,0}, 1},          /* {8}          */
    {2, {7,1,0}, 8},          /* {7,1}        */
    {2, {6,2,0}, 28},         /* {6,2}        */
    {2, {5,3,0}, 56},         /* {5,3}        */
    {3, {5,2,1}, 168},        /* {5,2,1}      */
    {3, {4,3,1}, 280}         /* {4,3,1}      */
};

seq_t a(idx_t n) {
    if (n < 8) return 0;   /* sequence is defined for n ≥ 8 */

    seq_t res = 0;

    for (int pi = 0; pi < 6; ++pi) {
        const Part *p = &parts[pi];

        if (p->k == 1) {                 /* multiplicity multiset {8} */
            if (n % 8 == 0) res += 1;
        } else if (p->k == 2) {          /* {m1,m2} */
            uint8_t m1 = p->m[0];
            uint8_t m2 = p->m[1];

            idx_t x_max = (n - 1) / m1;   /* m1*x < n must hold */
            for (idx_t x = 1; x <= x_max; ++x) {
                idx_t rem = n - m1 * x;
                if (rem % m2 != 0) continue;
                idx_t y = rem / m2;
                if (y == 0) continue;
                if (y == x) continue;     /* part sizes must differ */
                res += p->ratio;
            }
        } else if (p->k == 3) {          /* {m1,m2,m3} */
            uint8_t m1 = p->m[0];
            uint8_t m2 = p->m[1];
            uint8_t m3 = p->m[2];

            idx_t x_max = (n - 1) / m1;   /* ensure rem > 0 */
            for (idx_t x = 1; x <= x_max; ++x) {
                idx_t rem1 = n - m1 * x;
                if (rem1 <= 0) continue;

                idx_t y_max = (rem1 - 1) / m2;  /* ensure rem2 > 0 */
                for (idx_t y = 1; y <= y_max; ++y) {
                    idx_t rem2 = rem1 - m2 * y;
                    if (rem2 <= 0) continue;
                    if (rem2 % m3 != 0) continue;
                    idx_t z = rem2 / m3;
                    if (z == 0) continue;
                    if (z == x || z == y || x == y) continue;  /* distinct sizes */
                    res += p->ratio;
                }
            }
        }
    }

    return res;
}
