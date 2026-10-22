/*
 * A330350  Table of strictly decreasing sequences with terms in {0, ..., 9},
 *          sorted by length, then lexicographically.
 * https://oeis.org/A330350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = digit at position n of the concatenation of all strictly
 *            decreasing sequences of digits 0–9 (each sequence sorted
 *            lexicographically, sequences grouped by length)
 * Range:     u8 n <= 5120, u16 n <= 5120, u32 n <= 5120, u64 n <= 5120,
 *            u128 n <= 5120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes binomial coefficient C(n,k) as a seq_t.  All intermediate
   arithmetic is performed in seq_t to satisfy the arbitrary‑precision
   build.  The result always fits in idx_t for the small values used
   here, so it can safely be cast to idx_t when needed. */
static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; i++)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* There are only finitely many decreasing digit sequences.
       Total number of digits in the concatenation is 5120. */
    const idx_t TOTAL_DIGITS = (idx_t)5120;
    if (n == (idx_t)0 || n > TOTAL_DIGITS)
        return (seq_t)0;          /* out of range */

    /* Find the length L of the sequence that contains position n. */
    idx_t acc = (idx_t)0;
    idx_t len = (idx_t)0;
    for (idx_t l = (idx_t)1; l <= (idx_t)10; ++l)
    {
        idx_t block = l * (idx_t)binom_idx((idx_t)10, l);
        if (n <= acc + block)
        {
            len = l;
            break;
        }
        acc += block;
    }

    /* Position of n within the block of sequences of length len. */
    idx_t offset      = n - acc - (idx_t)1;          /* 0‑based */
    idx_t seq_index   = offset / len;               /* sequence index (0‑based) */
    idx_t pos_in_seq  = offset % len;               /* digit index in that sequence */

    /* Generate the seq_index‑th decreasing sequence of length len. */
    seq_t seq[10];          /* len ≤ 10 */
    idx_t remain = seq_index;
    idx_t prev_digit = (idx_t)10;   /* next digit must be < prev_digit */

    for (idx_t pos = (idx_t)0; pos < len; ++pos)
    {
        idx_t r = len - pos - (idx_t)1;          /* positions remaining after this one */
        idx_t dmin = r;                         /* need at least r smaller digits later */
        idx_t dmax = prev_digit - (idx_t)1;     /* must be < previous digit */

        for (idx_t d = dmin; d <= dmax; ++d)
        {
            seq_t cnt = binom_idx(d, r);
            idx_t c = (idx_t)cnt;   /* safe cast: cnt ≤ 252 */
            if (remain < c)
            {
                seq[pos] = (seq_t)d;
                prev_digit = d;
                break;
            }
            remain -= c;
        }
    }

    return seq[pos_in_seq];
}
