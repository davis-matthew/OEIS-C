/*
 * A291829  Numbers k such that k^3 is the sum of two positive 7th powers.
 * https://oeis.org/A291829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest of the two sequences A: 32 * m^7 (m ≥ 1) B:
 *            35723051649 * m^7 (m ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow7(seq_t x)
{
    return x * x * x * x * x * x * x;   /* x^7 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety, although offset = 1 */

    const seq_t baseA = (seq_t)32;
    const seq_t baseB = (seq_t)35723051649ULL;

    idx_t i = 1;         /* index in sequence A */
    idx_t j = 1;         /* index in sequence B */

    seq_t valA = baseA * pow7((seq_t)i);
    seq_t valB = baseB * pow7((seq_t)j);
    seq_t result = 0;

    for (idx_t cnt = 1; cnt <= n; ++cnt) {
        if (valB < valA) {
            result = valB;
            ++j;
            valB = baseB * pow7((seq_t)j);
        } else if (valA < valB) {
            result = valA;
            ++i;
            valA = baseA * pow7((seq_t)i);
        } else { /* valA == valB – avoid duplicate */
            result = valA;
            ++i;  ++j;
            valA = baseA * pow7((seq_t)i);
            valB = baseB * pow7((seq_t)j);
        }
    }
    return result;
}
