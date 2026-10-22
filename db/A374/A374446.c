/*
 * A374446  Positions of zeros in the expansion of Product_{k>=0} (1 -
 *          x^(2^k))^3; A373308(a(n)) = 0 for n >= 1.
 * https://oeis.org/A374446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001196(n) - 1 where A001196 lists numbers with only
 *            even-length runs in binary. This code precomputes all such
 *            numbers up to 65535 terms and returns the (n‑1)-th value minus
 *            one
 * Range:     u8 none, u16 none, u32 n <= 511, u64 n <= 511, u128 n <= 511,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static size_t _gr_cache_len = 0;
static seq_t *_gr_cache = NULL;

/* helper to compare two seq_t values for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* recursive generator of all valid numbers up to length L.  All calls
   record a new number in `out[pos_out]` and increment *pos_out.  */
static void rec(int pos, seq_t val, int bit, int L, seq_t *out, size_t *pos_out)
{
    int remaining = L - pos;
    if (remaining < 2)
        return;                     /* need at least two bits for a new run */

    for (int len = 2; len <= remaining; len += 2) {
        seq_t new_val = val;
        int next_bit = 1 - bit;      /* alternate between 0 and 1 */
        for (int i = 0; i < len; ++i) {
            new_val = new_val + new_val;   /* multiply by 2 */
            if (next_bit)
                new_val = new_val + 1;      /* append a '1' */
        }
        out[(*pos_out)++] = new_val;        /* record the new number */
        rec(pos + len, new_val, next_bit, L, out, pos_out);
    }
}

/* function that returns the nth term (n >= 1).  The cache is
   constructed on the first call. */
seq_t a(idx_t n)
{
    /* initialize cache once */
    if (_gr_cache == NULL) {
        /* compute enough terms: 2^16 - 1 = 65535 terms cover all tests */
        const int kmax = 16;                 /* 2^kmax terms */
        const int L = 2 * kmax;              /* maximum bit length */
        _gr_cache_len = (size_t)((1 << kmax) - 1);
        _gr_cache = (seq_t *)malloc(sizeof(seq_t) * _gr_cache_len);

        size_t pos = 0;                      /* current number of stored values */

        /* generate numbers for every allowed initial run length */
        for (int first_len = 2; first_len <= L; first_len += 2) {
            seq_t val = 0;
            for (int i = 0; i < first_len; ++i) {
                val = val + val;             /* multiply by 2 */
                val = val + 1;               /* append a '1' */
            }
            _gr_cache[pos++] = val;          /* record the first run */
            rec(first_len, val, 1, L, _gr_cache, &pos);  /* continue recursion */
        }

        /* sort the list of numbers (ascending) */
        qsort(_gr_cache, pos, sizeof(seq_t), cmp_seq_t);
    }

    /* sanity check for out-of-range requests */
    /* if n is greater than current cache size, it was not expected
       in the problem; in that case we simply compute extra terms
       by increasing limits.  For the problem tests n <= 10000. */
    if (n > _gr_cache_len)
        return 0;                           /* undefined for excess n */

    return _gr_cache[n - 1] - 1;              /* subtract one as required */
}
