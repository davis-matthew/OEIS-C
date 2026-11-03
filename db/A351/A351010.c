/*
 * A351010  Numbers k such that the k-th composition in standard order is a
 *          concatenation of twins (x,x).
 * https://oeis.org/A351010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th non‑negative integer k (starting at k=0) such
 *            that the composition obtained from k in standard order is a
 *            concatenation of twin blocks (x,x). a(1)=0. The function
 *            enumerates k in natural order and selects those whose
 *            composition satisfies the twin‑block property
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_twin(idx_t k) {
    /* A k satisfies the twin‑block property iff
       1. k has an even number of 1‑bits.
       2. For the 1‑bit positions p1 < p2 < ... < p_m (counted from 1),
          the gaps g_i = p_i - p_{i-1} (with p_0 = 0) satisfy
              g_1 = g_2, g_3 = g_4, ..., g_{m-1} = g_m  (m even). */
    if (k == 0)    /* empty composition – handled separately in a() */
        return false;

    int gaps[128];           /* maximal number of gaps for 64‑bit k */
    int gcnt = 0;
    int prev = -1;           /* previous 1‑bit position (1‑based) */
    idx_t tmp = k;

    while (tmp) {
        /* position of lowest set bit (0‑based) */
        idx_t pos = __builtin_ctzll(tmp);      /* pos in [0,63] */
        int p = (int)pos + 1;                  /* convert to 1‑based */

        if (prev == -1)
            gaps[gcnt++] = p;                  /* gap from 0 to first 1 */
        else
            gaps[gcnt++] = p - prev;           /* gap between consecutive 1s */

        prev = p;
        tmp &= (tmp - 1);                      /* clear lowest set bit */
    }

    if (gcnt % 2 != 0)           /* must have even number of parts */
        return false;

    /* check that gaps come in equal pairs */
    for (int i = 0; i < gcnt; i += 2) {
        if (gaps[i] != gaps[i + 1])
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 1u) /* offset 1: first term is 0 */
        return 0;

    idx_t found = 0;                /* count of twins found after 0 */
    for (idx_t k = 1; ; ++k) {      /* iterate over k = 1, 2, 3, ... */
        if (is_twin(k)) {
            ++found;
            if (found == n - 1)     /* n‑1 twins after the initial 0 */
                return (seq_t)k;
        }
    }
}
