/*
 * A276156  Numbers obtained by reinterpreting base-2 representation of n in
 *          primorial base: a(0) = 0, a(2n) = A276154(a(n)), a(2n+1) = 1 +
 *          A276154(a(n)).
 * https://oeis.org/A276156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum over i of bit_i(n) multiplied by primorial i, where
 *            primorial(0)=1 and primorial(i)=∏_{j=0}^{i-1} prime_j
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 1023, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t prim[65];
    static int initialized = 0;
    if (!initialized) {
        /* first 64 primes */
        static const unsigned primes[64] = {
            2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
            59,61,67,71,73,79,83,89,97,101,103,107,109,113,
            127,131,137,139,149,151,157,163,167,173,179,181,
            191,193,197,199,211,223,227,229,233,239,241,251,
            257,263,269,271,277,281,283,293,307,311
        };
        prim[0] = (seq_t)1;
        for (idx_t i = 1; i <= 64; ++i) {
            prim[i] = prim[i-1] * (seq_t)primes[i-1];
        }
        initialized = 1;
    }

    if (n == (idx_t)0) return (seq_t)0;

    seq_t result = (seq_t)0;
    idx_t pos = 0;
    while (n != 0) {
        if (n & (idx_t)1)
            result += prim[pos];
        n >>= 1;
        ++pos;
    }
    return result;
}
