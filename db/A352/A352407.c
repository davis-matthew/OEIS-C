/*
 * A352407  The number of terms before reaching zero when starting at n and
 *          iterating: f(n) = n, f(n+1) = n+1; f(n+k) = (f(n+k-2) * f(n+k-1))
 *          (mod (n+k)), where k>=2.
 * https://oeis.org/A352407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms before reaching zero when starting at n
 *            and n+1, with f(n) = n, f(n+1) = n+1 and for k ≥ 2 f(n+k) =
 *            (f(n+k-2) * f(n+k-1)) mod (n+k)
 * Range:     u8 n <= 70, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mul_mod(seq_t a, seq_t b, idx_t mod)
{
    /* Use 64‑bit intermediate multiplication; this is safe because
       a,b < 2^32 when n ≤ 10000 and mod fits in 64 bits. */
    unsigned long long prod = (unsigned long long)a * (unsigned long long)b;
    return (seq_t)(prod % (unsigned long long)mod);
}

seq_t a(idx_t n)
{
    /* Initial two terms of the sequence. */
    seq_t prev = n;          /* f(n)   */
    seq_t curr = n + 1;      /* f(n+1) */

    /* Count of terms before the first zero. */
    idx_t cnt = 2;           /* the two initial terms are always counted */

    /* Iterate until a zero appears. */
    for (idx_t k = 2; ; ++k)
    {
        idx_t mod = n + k;              /* modulus for this step */
        seq_t next = mul_mod(prev, curr, mod);

        if (next == 0)
            break;                      /* zero reached – stop counting */

        /* Advance the terms. */
        prev = curr;
        curr = next;
        ++cnt;
    }

    return cnt;
}
