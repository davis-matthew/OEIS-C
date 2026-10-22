/*
 * A341514  Number of trailing zeros in A097801-base.
 * https://oeis.org/A341514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n)=0 for odd n; for even n, a(n)=max{k: A(k)|n} - 1, where
 *            A(1)=1, A(2)=2 and A(k)=A(k-1)*(odd integer starting at 3)
 * Range:     u8 n <= 9943, u16 n <= 9943, u32 n <= 9943, u64 n <= 9943,
 *            u128 n <= 9943, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* odd indices have no trailing zeros */
    if ((n & 1ULL) != 0)
        return 0;

    idx_t cp   = 1;   /* current divisor A(k) */
    idx_t mult = 2;   /* next multiplier to build A(k+1) */
    idx_t k    = 1;   /* current k (A(1)=1 divides all even n) */

    /* loop as long as the next multiple fits into n and divides n */
    while (cp <= n / mult) {
        idx_t next = cp * mult;
        if (n % next != 0)
            break;
        cp   = next;
        mult = (mult == 2) ? 3 : mult + 2;   /* 2 → 3, then 5,7,9,… */
        k++;
    }

    return (seq_t)(k - 1);
}
