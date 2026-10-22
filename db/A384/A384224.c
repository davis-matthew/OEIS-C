/*
 * A384224  Irregular triangle read by rows: T(n,k) is the number of divisors
 *          in the k-th sublist of the divisors of n formed by the k-th odd
 *          divisor and the next even divisors that are less than the next odd
 *          divisor of n, with n >= 1, k >= 1.
 * https://oeis.org/A384224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the length of each sublist in the irregular triangle *
 *            read by rows as defined by OEIS A384224. For each integer m ≥ 1
 *            * (row m) its divisors are listed in increasing order. Whenever
 *            an odd * divisor occurs a new sublist is started, and all
 *            following even * divisors are appended to it unt...
 * Range:     u8 n <= 9554, u16 n <= 9554, u32 n <= 9554, u64 n <= 9554,
 *            u128 n <= 9554, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int cmp_uint64(const void *a, const void *b) {
    const uint64_t *va = (const uint64_t *)a;
    const uint64_t *vb = (const uint64_t *)b;
    return (*va > *vb) - (*va < *vb);
}

seq_t a(idx_t n) {
    if (n == 0) {
        /* undefined for index 0; return 0 as a safe value */
        return (seq_t)0;
    }

    idx_t produced = 0;          /* number of terms produced so far   */
    idx_t m = 1;                 /* current row number                */
    const size_t MAX_DIVS = 256; /* enough for 64‑bit numbers up to ~10⁶ */
    uint64_t divs[MAX_DIVS];
    size_t divcnt;

    while (produced < n) {
        /* ---- compute all divisors of m ---- */
        divcnt = 0;
        uint64_t mm = (uint64_t)m;
        for (uint64_t d = 1; d * d <= mm; ++d) {
            if (mm % d == 0) {
                if (divcnt < MAX_DIVS) divs[divcnt++] = d;
                uint64_t q = mm / d;
                if (q != d && divcnt < MAX_DIVS) divs[divcnt++] = q;
            }
        }
        qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

        /* ---- split into sublists ---- */
        seq_t cur_len = 0;
        for (size_t i = 0; i < divcnt; ++i) {
            uint64_t d = divs[i];
            if (d & 1ULL) {            /* odd divisor starts a new sublist */
                if (cur_len > 0) {     /* output previous sublist length */
                    ++produced;
                    if (produced == n) return cur_len;
                }
                cur_len = 1;           /* start new sublist with this divisor */
            } else {                   /* even divisor continues current sublist */
                ++cur_len;
            }
        }
        if (cur_len > 0) {             /* output last sublist of this row   */
            ++produced;
            if (produced == n) return cur_len;
        }

        ++m;
    }

    /* Should never reach here because produced == n in the loop */
    return (seq_t)0;
}
