/*
 * A349072  a(n) = T(n, 3*n), where T(n, x) is the Chebyshev polynomial of the
 *          first kind.
 * https://oeis.org/A349072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3*n + sqrt(9*n*n-1))^n + (3*n - sqrt(9*n*n-1))^n)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t seq_n = (seq_t)n;

    /* D = 9*n*n - 1 */
    seq_t D = (seq_t)9 * seq_n * seq_n - (seq_t)1;

    /* base element 3*n + sqrt(D) */
    seq_t cur_a = seq_n * (seq_t)3;        /* a part of the base */
    seq_t cur_b = (seq_t)1;                /* b part of the base */

    /* result initially 1 + 0*sqrt(D) */
    seq_t res_a = (seq_t)1;
    seq_t res_b = (seq_t)0;

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            /* res = res * cur */
            seq_t prod_a = res_a * cur_a;
            seq_t prod_b = res_b * cur_b;
            prod_b *= D;                  /* prod_b = res_b * cur_b * D */
            seq_t new_a = prod_a + prod_b;
            seq_t new_b = res_a * cur_b + res_b * cur_a;
            res_a = new_a;
            res_b = new_b;
        }
        exp >>= 1;
        if (!exp) break;

        /* cur = cur * cur */
        seq_t a_sq = cur_a * cur_a;
        seq_t b_sq = cur_b * cur_b;
        b_sq *= D;                          /* b_sq = cur_b * cur_b * D */
        seq_t new_a = a_sq + b_sq;

        seq_t tmp = cur_a * cur_b;           /* tmp = cur_a * cur_b */
        seq_t new_b = tmp + tmp;             /* 2 * cur_a * cur_b */

        cur_a = new_a;
        cur_b = new_b;
    }
    return res_a;
}
