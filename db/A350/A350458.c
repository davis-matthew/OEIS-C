/*
 * A350458  Chronological Julian day number of the first day (Tishri 1, Rosh
 *          Hashanah) of Hebrew calendar year n.
 * https://oeis.org/A350458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k(n) + 2 if k(n+1) - k(n) == 356 k(n) + 1 if k(n) -
 *            k(n-1) == 382 k(n) otherwise where m(n) = floor((n*235 -
 *            234)/19) j(n) = 347998 + floor((765433*m(n) + 12084)/25920) k(n)
 *            = j(n) + (floor(j(n)*6/7) mod 2) All operations are performed
 *            with integer arithmetic
 * Range:     u8 none, u16 none, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_k(idx_t n)
{
    /* m(n) = floor((n*235 - 234)/19) */
    seq_t m = ((seq_t)n * 235ULL - 234ULL) / 19ULL;

    /* j(n) = 347998 + floor((765433*m + 12084)/25920) */
    seq_t j = 347998ULL + (((seq_t)765433ULL * m + 12084ULL) / 25920ULL);

    /* k(n) = j + (floor(j*6/7) mod 2) */
    seq_t k = j + ((((j * 6ULL) / 7ULL) & 1ULL));

    return k;
}

seq_t a(idx_t n)
{
    seq_t k_n   = compute_k(n);
    seq_t k_next = compute_k(n + 1);

    /* Prefer the +2 adjustment first */
    if (k_next - k_n == 356ULL) {
        return k_n + 2ULL;
    }

    /* Then +1 adjustment, if previous exists */
    if (n > 1) {
        seq_t k_prev = compute_k(n - 1);
        if (k_n - k_prev == 382ULL) {
            return k_n + 1ULL;
        }
    }

    return k_n;
}
