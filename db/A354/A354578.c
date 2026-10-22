/*
 * A354578  Number of ways to choose a divisor of each part of the n-th
 *          composition in standard order such that no adjacent divisors are
 *          equal.
 * https://oeis.org/A354578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = number of ways to choose a divisor of each part of the
 *            n‑th composition (in standard order) such that no adjacent
 *            divisors are equal. The function generates that composition (via
 *            its binary representation) and then uses a simple DP over the
 *            divisors of each part
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The empty composition (n = 0) contributes exactly one
       way – choosing nothing. */
    if (n == 0)
        return (seq_t)1;

    /* Determine the length s of the composition whose index
       lies in n.  For compositions of length s there are 2^(s-1)
       of them, and the total up to length s is 2^s - 1.  */
    idx_t s = 1;
    idx_t power = 2;                     /* 2^s */
    while (power - (idx_t)1 < n) {
        ++s;
        power <<= 1;                     /* power = 2^s */
    }
    idx_t prev_power = power >> 1;       /* 2^(s-1)   */
    idx_t cnt_prev   = prev_power - (idx_t)1;  /* compositions before length s */
    idx_t k = n - cnt_prev - (idx_t)1;   /* zero‑based index within length‑s compositions */

    /* Reconstruct the composition of length s from the binary form of k.
       Each '0' bit extends the current part; each '1' starts a new part. */
    idx_t parts[20];                    /* s <= 14 for n <= 10000 – 20 is safe */
    int parts_len = 0;
    idx_t cur = 1;                      /* current part length */
    for (idx_t i = 0; i + (idx_t)1 < s; ++i) {
        idx_t bit = (k >> (s - (idx_t)2 - i)) & (idx_t)1;
        if (bit == 0)
            ++cur;
        else {
            parts[parts_len++] = cur;
            cur = 1;
        }
    }
    parts[parts_len++] = cur;            /* last part */

    /* DP arrays: counts[d] = number of assignments ending with divisor d.
       The maximum part value for the given n is <= s <= 14; 20 is sufficient. */
    const int MAXP = 20;
    seq_t counts[MAXP];
    seq_t new_counts[MAXP];

    /* Initialise counts to zero. */
    for (int d = 0; d < MAXP; ++d)
        counts[d] = (seq_t)0;

    /* First part: every divisor of the part is allowed as the first choice. */
    idx_t p0 = parts[0];
    for (idx_t d = (idx_t)1; d <= p0; ++d)
        if (p0 % d == (idx_t)0)
            counts[d] = (seq_t)1;

    /* Process remaining parts. */
    for (int idx = 1; idx < parts_len; ++idx) {
        idx_t p = parts[idx];

        /* Total number of assignments up to the previous part. */
        seq_t totalPrev = (seq_t)0;
        for (int d = 1; d < MAXP; ++d)
            totalPrev += counts[d];

        /* Reset new_counts. */
        for (int d = 1; d < MAXP; ++d)
            new_counts[d] = (seq_t)0;

        /* For each divisor d of the current part p, all previous
           assignments are valid except those ending with the same d. */
        for (idx_t d = (idx_t)1; d <= p; ++d)
            if (p % d == (idx_t)0) {
                seq_t val = totalPrev;
                if (counts[d] != (seq_t)0)
                    val -= counts[d];
                new_counts[d] = val;
            }

        /* Copy new_counts back to counts for next iteration. */
        for (int d = 1; d < MAXP; ++d)
            counts[d] = new_counts[d];
    }

    /* Sum all possible ending divisors to obtain the answer. */
    seq_t result = (seq_t)0;
    for (int d = 1; d < MAXP; ++d)
        result += counts[d];

    return result;
}
