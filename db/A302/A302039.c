/*
 * A302039  Analog of A056239 for nonstandard factorization based on the sieve
 *          of Eratosthenes (A083221).
 * https://oeis.org/A302039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, with r = A055396(n) (n lies in row r of the
 *            sieve array A083221) and c = A078898(n) (n is the c-th entry of
 *            that row): a(n) = r + a(c) + (r-1)*b(c), where b(1) = 0, b(n) =
 *            1 + b(c(n)) (b is the bigomega analog). This follows from a(n) =
 *            A056239(A250246(n)), where A250246(n) =...
 * Range:     u8 n <= 1618, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8791 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t a302039_lim = 0;
static uint64_t *a302039_val = NULL;   /* a(k) */

static void a302039_ensure(idx_t limit)
{
    if (limit <= a302039_lim) return;
    idx_t cap = a302039_lim ? a302039_lim : 1024;
    while (cap < limit) cap *= 2;

    uint32_t *ridx = (uint32_t *)calloc((size_t)cap + 1, sizeof(uint32_t));
    uint32_t *col  = (uint32_t *)calloc((size_t)cap + 1, sizeof(uint32_t));
    uint32_t *cnt  = (uint32_t *)calloc((size_t)cap + 2, sizeof(uint32_t));
    uint32_t *bo   = (uint32_t *)calloc((size_t)cap + 1, sizeof(uint32_t));
    uint64_t *val  = (uint64_t *)calloc((size_t)cap + 1, sizeof(uint64_t));
    if (!ridx || !col || !cnt || !bo || !val) {
        free(ridx); free(col); free(cnt); free(bo); free(val);
        return;
    }

    /* smallest-prime-factor index sieve */
    uint32_t np = 0;
    for (idx_t i = 2; i <= cap; ++i) {
        if (ridx[i] == 0) {
            ++np;
            for (idx_t j = i; j <= cap; j += i)
                if (ridx[j] == 0) ridx[j] = np;
        }
    }

    val[1] = 0; bo[1] = 0;
    for (idx_t k = 2; k <= cap; ++k) {
        uint32_t r = ridx[k];
        uint32_t c = ++cnt[r];          /* position of k within its row */
        col[k] = c;
        bo[k] = 1 + bo[c];
        val[k] = (uint64_t)r + val[c] + (uint64_t)(r - 1) * (uint64_t)bo[c];
    }

    free(ridx); free(col); free(cnt); free(bo);
    free(a302039_val);
    a302039_val = val;
    a302039_lim = cap;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    a302039_ensure(n);
    if (n > a302039_lim) return 0;
    return (seq_t)a302039_val[n];
}
