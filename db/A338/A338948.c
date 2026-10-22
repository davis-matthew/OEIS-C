/*
 * A338948  Number of oriented colorings of the 24 octahedral facets (or 24
 *          vertices) of the 4-D 24-cell using subsets of a set of n colors.
 * https://oeis.org/A338948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (96*n^2 + 144*n^3 + 48*n^4 + 44*n^6 + 36*n^7 * + 48*n^8 +
 *            36*n^9 + 33*n^12 + 72*n^13 + 18*n^14 * + n^24) / 576
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 8, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For built‑in integer types use 128‑bit intermediates when available.
       For arbitrary‑precision seq_t do the multiplication directly. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
        __uint128_t U = (unsigned __int128)n;
        __uint128_t n2 = U * U;
        __uint128_t n3 = n2 * U;
        __uint128_t n4 = n3 * U;
        __uint128_t n6 = n4 * n2;
        __uint128_t n7 = n6 * U;
        __uint128_t n8 = n7 * U;
        __uint128_t n9 = n8 * U;
        __uint128_t n12 = n9 * n3;
        __uint128_t n13 = n12 * U;
        __uint128_t n14 = n13 * U;
        __uint128_t n24 = n12 * n12;

        __uint128_t sum = 0;
        sum += 96u  * n2;
        sum += 144u * n3;
        sum += 48u  * n4;
        sum += 44u  * n6;
        sum += 36u  * n7;
        sum += 48u  * n8;
        sum += 36u  * n9;
        sum += 33u  * n12;
        sum += 72u  * n13;
        sum += 18u  * n14;
        sum += n24;
        sum /= 576u;
        return (seq_t)sum;
#else
        /* Fallback (may overflow for large n but n up to 8 already fits) */
        seq_t xn = (seq_t)n;
        seq_t n2 = xn * xn;
        seq_t n3 = n2 * xn;
        seq_t n4 = n3 * xn;
        seq_t n6 = n4 * n2;
        seq_t n7 = n6 * xn;
        seq_t n8 = n7 * xn;
        seq_t n9 = n8 * xn;
        seq_t n12 = n9 * n3;
        seq_t n13 = n12 * xn;
        seq_t n14 = n13 * xn;
        seq_t n24 = n12 * n12;

        seq_t sum = 0;
        sum += 96 * n2;
        sum += 144 * n3;
        sum += 48 * n4;
        sum += 44 * n6;
        sum += 36 * n7;
        sum += 48 * n8;
        sum += 36 * n9;
        sum += 33 * n12;
        sum += 72 * n13;
        sum += 18 * n14;
        sum += n24;
        sum /= 576;
        return sum;
#endif
    } else {
        /* Arbitrary‑precision seq_t: compute everything in seq_t */
        seq_t xn = (seq_t)n;
        seq_t n2 = xn * xn;
        seq_t n3 = n2 * xn;
        seq_t n4 = n3 * xn;
        seq_t n6 = n4 * n2;
        seq_t n7 = n6 * xn;
        seq_t n8 = n7 * xn;
        seq_t n9 = n8 * xn;
        seq_t n12 = n9 * n3;
        seq_t n13 = n12 * xn;
        seq_t n14 = n13 * xn;
        seq_t n24 = n12 * n12;

        seq_t sum = 0;
        sum += 96 * n2;
        sum += 144 * n3;
        sum += 48 * n4;
        sum += 44 * n6;
        sum += 36 * n7;
        sum += 48 * n8;
        sum += 36 * n9;
        sum += 33 * n12;
        sum += 72 * n13;
        sum += 18 * n14;
        sum += n24;
        sum /= 576;
        return sum;
    }
}
