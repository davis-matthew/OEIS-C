/*
 * A343317  a(n) is the least k >= 0 such that A343316(n, k) = n.
 * https://oeis.org/A343317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by taking the balanced‑ternary representation
 *            of n, * replacing each non‑zero digit by +1 and each zero digit
 *            by –1, * then interpreting the resulting digits as a base‑3
 *            number
 * Range:     u8 n <= 121, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pos = 0;      /* sum of positive contributions (digit'=+1) */
    seq_t neg = 0;      /* sum of negative contributions (digit'=-1) */
    seq_t pow = 1;      /* current power of 3, 3^i */
    uint64_t temp = n;  /* work on the index as a non‑negative integer */

    while (temp > 0)
    {
        uint64_t r = temp % 3;   /* r ∈ {0,1,2}  → digit in [0,1,-1] */
        temp /= 3;

        if (r == 0)             /* digit 0 → mapped to –1 */
        {
            neg += pow;
        }
        else                    /* digit 1 or –1 → mapped to +1 */
        {
            pos += pow;
            if (r == 2)          /* original digit –1: carry one to next position */
                temp += 1;
        }

        pow *= 3;               /* move to next higher power of 3 */
    }

    return pos - neg;           /* pos ≥ neg for all n (MSB is +1, lower powers cannot win) */
}
