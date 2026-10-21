/*
 * A289831  a(n) = A289813(n) + A289814(n).
 * https://oeis.org/A289831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n): binary digits correspond to non‑zero ternary digits of n.
 *            * a(0)=0. * For each n>0 write n in base‑3: n = sum_{k} d_k 3^k,
 *            d_k∈{0,1,2}. * Then a(n) = sum_{k} [d_k≠0] · 2^k
 * Range:     u8 n <= 6560, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t bit     = 1;
    while (n > 0)
    {
        if ((n % 3) != 0)
            result |= bit;
        bit <<= 1;
        n   /= 3;
    }
    return result;
}
