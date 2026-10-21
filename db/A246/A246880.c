/*
 * A246880  a(n) = 6*((10^n-1)/9)*(10^(n+1))+9*(10^n-1)/9.
 * https://oeis.org/A246880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation for OEIS A246880:
 * a(0) = 0, a(1) = 609, a(2) = 66099
 * a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3)   (n ≥ 3)          */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)609;
    if (n == 2) return (seq_t)66099;

    seq_t term0 = (seq_t)0;           /* a(n-3) */
    seq_t term1 = (seq_t)609;         /* a(n-2) */
    seq_t term2 = (seq_t)66099;       /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t new_term0 = term0 * (seq_t)1000;   /* 1000*a(n-3) */
        seq_t new_term1 = term1 * (seq_t)1110;   /* 1110*a(n-2) */
        seq_t new_term2 = term2 * (seq_t)111;    /* 111*a(n-1) */

        seq_t value = new_term2 + new_term0;     /* 111*a(n-1) + 1000*a(n-3) */
        value = value - new_term1;              /* - 1110*a(n-2) */

        term0 = term1;
        term1 = term2;
        term2 = value;
    }
    return term2;
}
