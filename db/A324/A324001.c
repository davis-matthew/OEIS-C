/*
 * A324001  Row 2 of array in A324000.
 * https://oeis.org/A324001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A324001: Row 2 of array in A324000.
   The sequence is periodic with period 8:
   a(0)=6, a(1)=6, a(2)=8, a(3)=3, a(4)=12,
   a(5)=3, a(6)=8, a(7)=6, then repeat. */

seq_t a(idx_t n)
{
    /* Use bitwise AND to compute n mod 8 (fast for power‑of‑two modulus). */
    switch ((unsigned)n & 7U) {
        case 0: return 6;
        case 1: return 6;
        case 2: return 8;
        case 3: return 3;
        case 4: return 12;
        case 5: return 3;
        case 6: return 8;
        case 7: return 6;
    }
    /* Unreachable, but keep compiler happy. */
    return 0;
}
