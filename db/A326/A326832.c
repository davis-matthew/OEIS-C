/*
 * A326832  Triangle read by rows: T(n, k) = (-1)^(n - k) * binomial(2n + 1, n
 *          - k) * L(2k + 1), 0 <= k <= n, where L(k) is the k-th Lucas number
 *          (A000032).
 * https://oeis.org/A326832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = flattened triangle T(n,k) where T(n,k) = (-1)^(n-k) *
 *            binomial(2n+1, n-k) * Lucas(2k+1)
 * Range:     i8 n <= 9, i16 n <= 27, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   binomial, free_seq, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/free_seq.h"

/* static cache for Lucas numbers */
static seq_t *lucas_cache = NULL;
static idx_t lucas_len = 0;

/* ensure Lucas numbers up to index 'need' are computed */
static void ensure_lucas(idx_t need)
{
    if (lucas_len > need) return;

    seq_t *new_arr = seq_realloc(lucas_cache, lucas_len, need + 1);
    /* compute missing entries */
    for (idx_t i = 0; i <= need; ++i) {
        if (i == 0) {
            new_arr[0] = 2;
        } else if (i == 1) {
            new_arr[1] = 1;
        } else {
            new_arr[i] = new_arr[i - 1] + new_arr[i - 2];
        }
    }
    lucas_cache = new_arr;
    lucas_len   = need + 1;
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t start = r * (r + 1) / 2;
    idx_t k     = n - start;   /* column index */

    /* need Lucas number L(2k+1) */
    ensure_lucas(2 * k + 1);
    seq_t lucas_val = lucas_cache[2 * k + 1];

    /* compute binomial coefficient */
    seq_t bin = binomial(2 * r + 1, r - k);

    /* combine with sign */
    seq_t result = bin * lucas_val;
    if ((r - k) & 1) {
        result = -result;
    }
    return result;
}
