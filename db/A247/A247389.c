/*
 * A247389  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123456789)*.
 * https://oeis.org/A247389
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = - 9*ord₉(n) if gcd(n,9) = 1, - 10 if 9 | n, - 13 when n =
 *            3, - 16 when n = 6, - 19 when n ≡ 3 or 6 (mod 9) and n ≥ 12
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < (idx_t)2) return (seq_t)0;          /* undefined for n < 2 */

    idx_t m9 = n % (idx_t)9;
    switch (m9)
    {
        case 0: return (seq_t)10;                /* 9 | n  -> a(n)=10 */
        case 1: return (seq_t)9;                 /* ord₉(n)=1 -> 9*1 = 9 */
        case 2: return (seq_t)54;                /* ord₉(n)=6 -> 9*6 = 54 */
        case 3: break;                           /* handled below */
        case 4: return (seq_t)27;                /* ord₉(n)=3 -> 9*3 = 27 */
        case 5: return (seq_t)54;                /* ord₉(n)=6 -> 9*6 = 54 */
        case 6: break;                           /* handled below */
        case 7: return (seq_t)27;                /* ord₉(n)=3 -> 9*3 = 27 */
        case 8: return (seq_t)18;                /* ord₉(n)=2 -> 9*2 = 18 */
    }

    /* at this point m9 is 3 or 6 (n divisible by 3 but not by 9) */
    if (n == (idx_t)3) return (seq_t)13;
    if (n == (idx_t)6) return (seq_t)16;
    return (seq_t)19;
}
