/*
 * A256433  Characteristic function of dodecahedral numbers.
 * https://oeis.org/A256433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a dodecahedral number, 0 otherwise. A
 *            dodecahedral number is m(3m-1)(3m-2)/2 for some integer m ≥ 0
 * Range:     u8 n <= 623, u16 n <= 105994, u32 n <= 105995, u64 n <= 105995,
 *            u128 n <= 105995, big any n
 * Validated: standard, pass, 101 terms through n = 8787 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t dodeca_value(idx_t m)
{
    /* Compute f(m) = m(3m-1)(3m-2)/2 = (9m^3 - 9m^2 + 2m)/2 */
    seq_t mm = (seq_t)m;
    seq_t m2 = mm * mm;
    seq_t m3 = m2 * mm;
    seq_t term1 = (seq_t)9 * m3; /* 9m^3 */
    seq_t term2 = (seq_t)9 * m2; /* 9m^2 */
    seq_t term3 = (seq_t)2 * mm; /* 2m */
    return (term1 - term2 + term3) / (seq_t)2;
}

seq_t a(idx_t n)
{
    /* Binary search for m such that dodeca_value(m) == n. */
    idx_t low = (idx_t)0;
    idx_t high = (idx_t)1;

    /* Find an upper bound *high* such that dodeca_value(high) >= n. */
    while (dodeca_value(high) < (seq_t)n)
        high <<= 1;                      /* double high */

    /* Binary search in [low, high] */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t val = dodeca_value(mid);
        if (val < (seq_t)n)
            low = mid + 1;
        else
            high = mid;
    }

    /* low now holds the smallest m with dodeca_value(m) >= n. */
    return (dodeca_value(low) == (seq_t)n) ? (seq_t)1 : (seq_t)0;
}
