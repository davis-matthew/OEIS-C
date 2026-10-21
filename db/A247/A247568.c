/*
 * A247568  Base-7 state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...n)*.
 * https://oeis.org/A247568
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n * ord_n(7) if 7 and n are coprime, a(n) = n + 1 if 7
 *            divides n, where ord_n(7) is the smallest positive k with 7^k ≡
 *            1 (mod n)
 * Range:     u8 n <= 16, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 2 */
    if (n % 7 == 0) {
        return (seq_t)(n + 1);
    }

    /* Compute the multiplicative order of 7 modulo n */
    idx_t mod = n;
    idx_t pow = 7 % mod;
    idx_t ord = 1;

    while (pow != 1) {
        pow = (pow * 7) % mod;
        ord++;
        /* In theory ord ≤ φ(n) < n, but guard against infinite loop */
        if (ord > mod) break;
    }

    return (seq_t)n * (seq_t)ord;
}
