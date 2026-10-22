/*
 * A343248  Number of partitions of n into 3 parts such that no part divides
 *          n.
 * https://oeis.org/A343248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 positive parts
 *            (x>=y>=z>=1) such that none of the parts divides n. Computed by
 *            brute‑force enumeration
 * Range:     u8 n <= 58, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static unsigned char *divflag = NULL;
    static size_t divcap = 0;
    size_t need = (size_t)(n + 1);
    if (need > divcap) {
        unsigned char *tmp = (unsigned char*)realloc(divflag, need);
        if (!tmp) exit(1);
        divflag = tmp;
        divcap = need;
    }

    for (idx_t d = 1; d <= n; ++d)
        divflag[(size_t)d] = (n % d == 0) ? 1 : 0;

    seq_t count = 0;
    idx_t zmax = n / 3;
    for (idx_t z = 1; z <= zmax; ++z) {
        if (divflag[(size_t)z]) continue;      /* z divides n → invalid */
        idx_t ymax = (n - z) / 2;              /* ensure x >= y >= z */
        for (idx_t y = z; y <= ymax; ++y) {
            if (divflag[(size_t)y]) continue;  /* y divides n → invalid */
            idx_t x = n - y - z;               /* x >= y by construction */
            if (divflag[(size_t)x]) continue;  /* x divides n → invalid */
            ++count;
        }
    }
    return count;
}
