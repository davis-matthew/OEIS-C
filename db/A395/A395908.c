/*
 * A395908  Series expansion of (27*x - 14*x^2 + 2*x^3) / (1 - x)^7.
 * https://oeis.org/A395908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+5)*(n+8) / 48
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 73, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n == 0 separately – the product formula yields 0 as desired. */
    if (n == 0)
        return 0;

#if defined(__SIZEOF_INT128__)
    /* 128‑bit intermediate to avoid overflow for any seq_t up to 128 bits. */
    unsigned __int128 prod = 1;
    prod *= (unsigned __int128)n;
    prod *= (unsigned __int128)(n + 1);
    prod *= (unsigned __int128)(n + 2);
    prod *= (unsigned __int128)(n + 3);
    prod *= (unsigned __int128)(n + 5);
    prod *= (unsigned __int128)(n + 8);
    prod /= 48;
    return (seq_t)prod;
#else
    /* Fallback to a wide native type; almost all compilers support 64‑bit. */
    unsigned long long prod = 1ull;
    prod *= (unsigned long long)n;
    prod *= (unsigned long long)(n + 1);
    prod *= (unsigned long long)(n + 2);
    prod *= (unsigned long long)(n + 3);
    prod *= (unsigned long long)(n + 5);
    prod *= (unsigned long long)(n + 8);
    prod /= 48;
    return (seq_t)prod;
#endif
}
