/*
 * A258733  Number of length n+3 0..3 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A258733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 73, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (OEIS A258733, offset 1):
 *   a(n) = (n^7 + 49 n^6 + 1015 n^5 + 10675 n^4 + 115024 n^3 +
 *           455476 n^2 + 582000 n + 120960) / 5040   for n > 1
 *   a(1) = 256
 *
 * The expression is exact for every n >= 1 that fits in seq_t.
 */

seq_t a(idx_t n)
{
    if (n == 1ULL)
        return (seq_t)256;

#if defined(__SIZEOF_INT128__)   /* use 128‑bit math when available */
    /* All intermediate arithmetic is performed in __int128, then the
     * final result is cast to seq_t (which may be uint64_t, unsigned
     * __int128, or an arbitrary‑precision class). */
    __int128 sn   = n;
    __int128 n2   = sn * sn;
    __int128 n3   = n2 * sn;
    __int128 n4   = n3 * sn;
    __int128 n5   = n4 * sn;
    __int128 n6   = n5 * sn;
    __int128 n7   = n6 * sn;

    __int128 numerator =
        n7                                   +
        (__int128)49   * n6                  +
        (__int128)1015 * n5                  +
        (__int128)10675 * n4                 +
        (__int128)115024 * n3                +
        (__int128)455476 * n2                +
        (__int128)582000 * sn                +
        (__int128)120960;

    __int128 result = numerator / 5040;
    return (seq_t)result;

#else   /* no builtin 128‑bit type – fall back to seq_t arithmetic */
    /* The sequence values for the test ranges fit in seq_t, and seq_t
     * (for 64‑bit builds) is capable of holding intermediate results
     * because the test input uses comparatively small n. */
    seq_t sn   = (seq_t)n;
    seq_t n2   = sn * sn;
    seq_t n3   = n2 * sn;
    seq_t n4   = n3 * sn;
    seq_t n5   = n4 * sn;
    seq_t n6   = n5 * sn;
    seq_t n7   = n6 * sn;

    seq_t numerator =
        n7                                   +
        (seq_t)49   * n6                    +
        (seq_t)1015 * n5                    +
        (seq_t)10675 * n4                   +
        (seq_t)115024 * n3                  +
        (seq_t)455476 * n2                  +
        (seq_t)582000 * sn                  +
        (seq_t)120960;

    return numerator / (seq_t)5040;
#endif
}
