/*
 * A241592  Degree of Somos4-oid polynomial s_n(x).
 * https://oeis.org/A241592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   deg(s_{8q+r}) = (4q + r)q + [-2,0,0,0,1,1,2,3]_r, 0 <= r <= 7
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n≥1. */
seq_t a(idx_t n)
{
    idx_t q = n / 8;
    idx_t r = n % 8;
    seq_t qst = (seq_t)q;
    seq_t a_temp = (seq_t)((seq_t)4 * qst + (seq_t)r);
    seq_t result = a_temp * qst;
    if (r == 0) {
        result -= (seq_t)2;
    } else if (r == 4 || r == 5) {
        result += (seq_t)1;
    } else if (r == 6) {
        result += (seq_t)2;
    } else if (r == 7) {
        result += (seq_t)3;
    }
    return result;
}
