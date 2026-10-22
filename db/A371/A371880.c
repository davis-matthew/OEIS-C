/*
 * A371880  Smallest number reachable starting from 1 and taking n steps
 *          either doubling or doubling+reversing.
 * https://oeis.org/A371880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest positive residue mod 99 of 2^n and 10*2^n,
 *            except a(9) = 71
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 9) return 71;  /* special case */
    idx_t e = n;
    seq_t result = 1;       /* 2^0 mod 99 */
    seq_t base = 2;        /* 2^1 mod 99 */
    const seq_t mod = 99;
    while (e > 0) {
        if (e & 1ULL) result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    seq_t alt = (10 * result) % mod;
    return result < alt ? result : alt;
}
