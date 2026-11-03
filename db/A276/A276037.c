/*
 * A276037  Numbers using only digits 1 and 5.
 * https://oeis.org/A276037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(2)=5 Recurrence: a(2n+1) = 10*a(n) + 1 a(2n+2) =
 *            10*a(n) + 5 (valid for n ≥ 1, with n=0 treated as 0)
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1534, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Validated: standard, pass, 60 terms through n = 7345 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _a_calc(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)5;

    /* parent index in the binary‑heap representation */
    seq_t parent = _a_calc((n - 1) / 2);

    if (n & 1)          /* odd child → append digit '1' */
        return parent * ((seq_t)10) + ((seq_t)1);
    else                /* even child → append digit '5' */
        return parent * ((seq_t)10) + ((seq_t)5);
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1; return 0 for safety */
        return (seq_t)0;
    return _a_calc(n);
}
