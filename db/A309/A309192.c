/*
 * A309192  a(n) = Sum_{k=1..n} mu(k)^2 * k * floor(n/k).
 * https://oeis.org/A309192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(k)^2 * k * floor(n/k)
 * Range:     u8 n <= 21, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* mu(k)^2 is 1 on square‑free k, 0 otherwise.  This
   implementation pre‑computes a square‑free table by
   marking multiples of p² for every p up to √n.  The table
   is cached and grown as needed, so repeated or
   unordered calls remain O(n). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache of square‑free status: 1 = square‑free, 0 = not square‑free. */
static uint8_t *sqfree_tbl = NULL;
static size_t   tbl_size   = 0;   /* current number of entries in sqfree_tbl (n+1). */

/* Integer square‑root floor for unsigned 64‑bit values. */
static idx_t integer_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid == 0) { ans = 0; lo = 1; continue; }
        if (mid > n / mid) hi = mid - 1;
        else               { ans = mid; lo = mid + 1; }
    }
    return ans;
}

/* Grow the square‑free table to at least required (inclusive). */
static void ensure_sqfree_table(idx_t required)
{
    if (required + 1 <= tbl_size) return;          /* already large enough */

    size_t new_size = (size_t)required + 1;          /* include index 'required' */
    uint8_t *new_tbl = (uint8_t *)calloc(new_size, 1);   /* all zero */
    if (!new_tbl) return;                           /* allocation failed */

    /* Initially mark every number (except 0) as square‑free. */
    for (size_t i = 1; i < new_size; ++i) new_tbl[i] = 1;
    new_tbl[0] = 0;                                 /* 0 is not used */

    /* Mark multiples of p² as NOT square‑free. */
    idx_t lim = integer_sqrt(required);
    for (idx_t p = 2; p <= lim; ++p) {
        idx_t psq = p * p;
        for (idx_t j = psq; j <= required; j += psq)
            new_tbl[j] = 0;
    }

    free(sqfree_tbl);
    sqfree_tbl = new_tbl;
    tbl_size   = new_size;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for out‑of‑range calls */

    ensure_sqfree_table(n);

    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (sqfree_tbl[k]) {
            idx_t q = n / k;
            total += (seq_t)k * (seq_t)q;
        }
    }
    return total;
}
