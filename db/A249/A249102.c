/*
 * A249102  Numbers with no 1's in base-7 expansion.
 * https://oeis.org/A249102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = interpret (n-1) as a base‑6 number with digits 0‑5.
 *   Map each digit d: 0 → 0, 1‑5 → (d+1) (i.e. 2‑6).  
 * The resulting digits form a base‑7 number; its decimal value is a(n).
 * This yields the sequence of integers with no 1's in base‑7 expansion.
 */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t place  = 1;   /* current power of 7 (7^k) */
    idx_t m      = n - 1; /* zero‑based index */

    while (m > 0) {
        uint64_t d = m % 6ULL;           /* base‑6 digit */
        seq_t   mapped = (d == 0) ? 0 : (seq_t)(d + 1); /* 0→0, 1→2, ..., 5→6 */
        result += mapped * place;
        place *= (seq_t)7;
        m /= 6ULL;
    }
    return result; /* n==1 gives m==0 → result==0 as required */
}
