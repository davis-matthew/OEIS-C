/*
 * A346707  "Look once to the left" sequence, omitting a(k) for each iteration
 *          k, starting with 1,2 (see example).
 * https://oeis.org/A346707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A346707 – "Look once to the left"
   a(1) = 1,  a(2) = 2.
   For n > 2:
     find k = ceil(log₂(n‑1))  (smallest k ≥ 0 with 2^k ≥ n‑1)
     let L = 2^(k‑1) + 1               // length before the appended block
     pos = n – L                       // 1 … 2^(k‑1)
     if pos < k   →   n = pos
     else        →   n = pos + 1
   Repeat until n ≤ 2.  The returned value is then 1 or 2. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, safety */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    idx_t cur = n;
    while (cur > 2)
    {
        /* find smallest k with 2^k >= cur-1 */
        idx_t target = cur - 1;        /* cur > 2 ⇒ target ≥ 2 */
        idx_t k = 0;
        idx_t pow = 1;
        while (pow < target)
        {
            pow <<= 1;
            ++k;
        }

        /* length before the appended block: 2^(k-1) + 1  (k ≥ 1) */
        idx_t len_before = ( (idx_t)1 << (k - 1) ) + 1;
        idx_t pos = cur - len_before;  /* 1 … 2^(k-1) */

        if (pos < k)
            cur = pos;
        else
            cur = pos + 1;
    }

    return (cur == 1) ? (seq_t)1 : (seq_t)2;
}
