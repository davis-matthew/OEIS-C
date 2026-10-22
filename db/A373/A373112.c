/*
 * A373112  Number of binary digits in the integer part of e^n.
 * https://oeis.org/A373112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n*log2(e)) + 1
 * Range:     u8 n <= 176, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset; values for n < 1 are not specified */
    if (n == 0)
        return 0;

    /* 2^64 scaling factor for fixed‑point arithmetic */
    static const unsigned __int128 SCALE = ((unsigned __int128)1) << 64;

    /* log2(e) expressed as a 128‑bit fixed point (scaled by 2^64) */
    static const unsigned __int128 LOG2E_Q =
        (__int128)((long double)1.0L / 0.6931471805599453094172321214581765680755L *
                   SCALE + 0.5L);

    /* Compute floor(n * log2(e)) as the high 64 bits of the product */
    unsigned __int128 prod = ((unsigned __int128)n) * LOG2E_Q;
    unsigned long long k   = (unsigned long long)(prod >> 64);

    return (seq_t)k + 1;
}
