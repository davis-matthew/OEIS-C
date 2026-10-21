/*
 * A250688  Number of (n+1)X(5+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 24, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A250688 – number of (n+1)×6 arrays with given constraints
    Recurrence (for n > 17):
        a(n) = 27 a(n-1) – 331 a(n-2) + 2439 a(n-3) – 12049 a(n-4)
              + 42129 a(n-5) – 107225 a(n-6) + 201069 a(n-7) – 277706 a(n-8)
              + 278928 a(n-9) – 197984 a(n-10) + 94032 a(n-11) – 26784 a(n-12)
              + 3456 a(n-13)
    Initial values:
        a(1) = 47002
        a(2) = 249512
        a(3) = 1315446
        a(4) = 6671458
        a(5) = 31728758
        a(6) = 143136866
        a(7) = 617657415
        a(8) = 2571646112
        a(9) = 10412478449
        a(10) = 41269021168
        a(11) = 160968910027
        a(12) = 620573782940
        a(13) = 2373117015149
        a(14) = 9027885707796
        a(15) = 34248252841075
        a(16) = 129817387985168
        a(17) = 492449247898701
    The function works for any n ≥ 1, calling it in any order. */

#include <stdbool.h>
#include <stddef.h>

static const seq_t init[17] = {
    47002ULL,          /* a(1) */
    249512ULL,         /* a(2) */
    1315446ULL,        /* a(3) */
    6671458ULL,        /* a(4) */
    31728758ULL,       /* a(5) */
    143136866ULL,      /* a(6) */
    617657415ULL,      /* a(7) */
    2571646112ULL,     /* a(8) */
    10412478449ULL,    /* a(9) */
    41269021168ULL,    /* a(10) */
    160968910027ULL,   /* a(11) */
    620573782940ULL,   /* a(12) */
    2373117015149ULL,  /* a(13) */
    9027885707796ULL,  /* a(14) */
    34248252841075ULL, /* a(15) */
    129817387985168ULL,/* a(16) */
    492449247898701ULL /* a(17) */
};

static const int32_t coeff[13] = {
    27, -331, 2439, -12049, 42129, -107225,
    201069, -277706, 278928, -197984, 94032,
    -26784, 3456
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset 1 – not used */
    if (n <= 17) return init[n - 1];

    /* buffer holds a(k-13) … a(k-1) for current k; we start with a(5)…a(17) */
    seq_t buf[13];
    for (idx_t i = 0; i < 13; ++i) buf[i] = init[4 + i];   /* a(5) at buf[0] ... a(17) at buf[12] */

    seq_t term = 0;
    for (idx_t k = 18; k <= n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int j = 0; j < 13; ++j) {
            int32_t c = coeff[j];
            /* buf[12-j] is the term that coeff[j] multiplies in the recurrence */
            seq_t prod = buf[12 - j] * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0) {
                sum_pos += prod;
            } else {
                sum_neg += prod;
            }
        }
        term = sum_pos - sum_neg;  /* safe: sum_pos ≥ sum_neg for valid recurrence */

        if (k == n) return term;

        /* shift buffer left: drop oldest, add new term */
        for (int j = 0; j < 12; ++j) buf[j] = buf[j + 1];
        buf[12] = term;
    }
    return term;  /* unreachable */
}
