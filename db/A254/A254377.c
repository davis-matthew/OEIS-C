/*
 * A254377  Characteristic function of A230709: a(n) = 1 if n is either evil
 *          (A001969) or even odious (A128309), otherwise 0 (when n is odd
 *          odious).
 * https://oeis.org/A254377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if popcount(n) is even (evil) or if n is even odious;
 *            otherwise a(n) = 0. This is equivalent to the definition: 1 if n
 *            is evil (even number of 1's in binary) or n is even and odious
 *            (odd number of 1's)
 * Range:     u8 n <= 9995, u16 n <= 9995, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned int pc;

#ifdef __GNUC__
    /* Fast popcount using compiler builtin */
    pc = __builtin_popcountll((unsigned long long)n);
#else
    /* Portable popcount implementation */
    idx_t tmp = n;
    pc = 0;
    while (tmp) {
        pc += tmp & 1;
        tmp >>= 1;
    }
#endif

    if ((pc & 1) == 0)          /* even popcount → evil */
        return (seq_t)1;
    if ((n & 1) == 0)           /* odd popcount but n even → even odious */
        return (seq_t)1;
    return (seq_t)0;            /* odd popcount and n odd → odd odious */
}
