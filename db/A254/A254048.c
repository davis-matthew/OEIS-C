/*
 * A254048  a(n) = A126760(A007494(n)).
 * https://oeis.org/A254048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A126760(A007494(n)), A007494(n) = 3*floor(n/2) + 2*(n mod
 *            2); A126760: a(0)=0, a(2n)=a(n), a(3n)=a(n), a(6n+1)=2n+1,
 *            a(6n+5)=2n+2
 * Range:     u8 n <= 509, u16 n <= 19683, u32 n <= 19683, u64 n <= 19683,
 *            u128 n <= 19683, big any n
 * Validated: standard, pass, 137 terms through n = 9965 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A007494_a(idx_t n)
{
    return (seq_t)3 * (seq_t)(n / 2) + (seq_t)2 * (seq_t)(n % 2);
}

static seq_t A126760_a(seq_t m)
{
    seq_t result = 0;
    while (m > 0) {
        seq_t r = m % 6;
        if (r == 1) {
            result = (seq_t)2 * ((m - 1) / 6) + 1;
            break;
        }
        if (r == 5) {
            result = (seq_t)2 * ((m - 5) / 6) + 2;
            break;
        }
        if (r % 2 == 0) {
            m /= 2;
            continue;
        }
        if (m % 3 == 0) {
            m /= 3;
            continue;
        }
        break;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A126760_a(A007494_a(n));
}
