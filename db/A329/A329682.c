/*
 * A329682  Number of excursions of length n with Motzkin-steps forbidding all
 *          consecutive steps of length 2 except UH, UD, HU and DD.
 * https://oeis.org/A329682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = a(n-3) for n > 4 (a(0) = a(1) = a(2) = a(3) = 1, a(4) =
 *            0)
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly to avoid boundary problems */
    switch (n) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)1;
    case 2: return (seq_t)1;
    case 3: return (seq_t)1;
    case 4: return (seq_t)0;
    }

    /* For n >= 5 the sequence repeats every 3 steps:
       indices 0, 2 mod 3 → 1, index 1 mod 3 → 0 */
    switch (n % 3) {
    case 0: return (seq_t)1; /* n ≡ 0 (mod 3) → 1 */
    case 1: return (seq_t)0; /* n ≡ 1 (mod 3) → 0 */
    case 2: return (seq_t)1; /* n ≡ 2 (mod 3) → 1 */
    }

    /* Unreachable, but keeps the compiler happy */
    return (seq_t)0;
}
