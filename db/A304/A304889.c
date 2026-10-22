/*
 * A304889  Number of nX3 0..1 arrays with every element unequal to 0, 2, 3, 4
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence from the OEIS entry (valid for n > 20).
   Initial values a(1)..a(20) are given below. */
seq_t a(idx_t n)
{
    /* Initial terms a(1) .. a(20) */
    static const seq_t init[20] = {
        1ULL,        /* a(1) */
        8ULL,        /* a(2) */
        11ULL,       /* a(3) */
        40ULL,       /* a(4) */
        94ULL,       /* a(5) */
        291ULL,      /* a(6) */
        776ULL,      /* a(7) */
        2269ULL,     /* a(8) */
        6275ULL,     /* a(9) */
        17985ULL,    /* a(10) */
        50548ULL,    /* a(11) */
        143589ULL,   /* a(12) */
        405620ULL,   /* a(13) */
        1149709ULL,  /* a(14) */
        3252139ULL,  /* a(15) */
        9211083ULL,  /* a(16) */
        26070305ULL, /* a(17) */
        73816122ULL, /* a(18) */
        208963020ULL,/* a(19) */
        591618663ULL /* a(20) */
    };

    if (n <= 20) return init[n - 1];

    /* Coefficients of the linear recurrence for n > 20.
       coef[i] corresponds to a(n-(i+1)). */
    static const int coef[19] = {
        3,   /* a(n-1) */
        3,   /* a(n-2) */
        -7,  /* a(n-3) */
        -8,  /* a(n-4) */
        -8,  /* a(n-5) */
        26,  /* a(n-6) */
        -13, /* a(n-7) */
        -16, /* a(n-8) */
        76,  /* a(n-9) */
        -33, /* a(n-10) */
        -22, /* a(n-11) */
        27,  /* a(n-12) */
        -23, /* a(n-13) */
        11,  /* a(n-14) */
        7,   /* a(n-15) */
        -5,  /* a(n-16) */
        5,   /* a(n-17) */
        -2,  /* a(n-18) */
        -2   /* a(n-19) */
    };

    /* Circular buffer holding the previous 19 terms:
       prev[0] = a(k-19), ..., prev[18] = a(k-1) during iteration of k. */
    seq_t prev[19];

    /* For k = 21 we need a(20) .. a(2) in decreasing order.
       Initialize prev[0] = a(2), ..., prev[18] = a(20). */
    for (idx_t i = 0; i < 19; ++i) {
        prev[i] = init[i + 1];
    }

    seq_t result = 0;   /* placeholder for the current a(k) */

    for (idx_t k = 21; k <= n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* sum over i = 0..18  (corresponding to a(k-(i+1))) */
        for (int i = 0; i < 19; ++i) {
            seq_t val = prev[18 - i];
            if (coef[i] > 0) {
                sum_pos += (seq_t)coef[i] * val;
            } else {
                sum_neg += (seq_t)(-coef[i]) * val;
            }
        }
        result = sum_pos - sum_neg;   /* a(k) */

        /* Shift the circular buffer: discard a(k-19), append a(k). */
        for (idx_t j = 0; j < 18; ++j) {
            prev[j] = prev[j + 1];
        }
        prev[18] = result;
    }

    return result;   /* a(n) */
}
