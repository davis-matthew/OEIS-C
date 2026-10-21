/*
 * A294053  Number of set partitions of [n] such that the maximal absolute
 *          difference between consecutive elements within a block equals
 *          four.
 * https://oeis.org/A294053
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A294053: number of set partitions of [n] such that the maximal 
    absolute difference between consecutive elements within a block equals 4.
    The generating function is
        G(x) = (x^5-2*x^4+x^3-3*x^2+7*x-5)*x^5 /
               ((x-1)*(x^3-x^2-3*x+1)*(x^8 -x^7 -7*x^5 +7*x^4 +x^3 +4*x^2 -5*x+1)).
    From the denominator we derive the linear recurrence
        a(n) = 9 a(n-1) -26 a(n-2) +23 a(n-3) +20 a(n-5) -40 a(n-6) +2 a(n-7)
               +16 a(n-8) -1 a(n-9) -3 a(n-11) +1 a(n-12)
    for all n ≥ 17  (offset 5). Initial values a(5)…a(16) are required. 
 */

/* Initial terms a(5) … a(16) */
static const seq_t init[12] = {
    /* a(5)  */ (seq_t)5,
    /* a(6)  */ (seq_t)38,
    /* a(7)  */ (seq_t)215,
    /* a(8)  */ (seq_t)1061,
    /* a(9)  */ (seq_t)4835,
    /* a(10) */ (seq_t)20973,
    /* a(11) */ (seq_t)88010,
    /* a(12) */ (seq_t)360787,
    /* a(13) */ (seq_t)1453978,
    /* a(14) */ (seq_t)5784863,
    /* a(15) */ (seq_t)22790024,
    /* a(16) */ (seq_t)89092968
};

/* Recurrence coefficients for a(n) = Σ coeff_k * a(n-k)  (k=1..12) */
static const int coeff[12] = {
    9,   /* a(n-1) */
   -26,  /* a(n-2) */
    23,  /* a(n-3) */
     0,  /* a(n-4) */
    20,  /* a(n-5) */
   -40,  /* a(n-6) */
     2,  /* a(n-7) */
    16,  /* a(n-8) */
    -1,  /* a(n-9) */
     0,  /* a(n-10) */
    -3,  /* a(n-11) */
     1   /* a(n-12) */
};

/* Internal circular buffer for the last 12 terms; initialized lazily. */
static seq_t buffer[12];
static idx_t built_up = (idx_t)(-1); /* number of terms computed so far (n-5) */

/* Utility to compute a new term from the 12 recent ones. */
static seq_t recurrence_step(idx_t idx)
{
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    for (int k = 0; k < 12; ++k) {
        /* index of a(idx - (k+1)) */
        idx_t off = idx + 12 - (k + 1);
        off %= 12;
        seq_t val = buffer[off];
        int c   = coeff[k];
        seq_t prod = val * (seq_t)(c >= 0 ? c : -c);
        if (c >= 0) {
            pos_sum += prod;
        } else {
            neg_sum += prod;
        }
    }
    return pos_sum - neg_sum;
}

seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;      /* not defined for offset < 5 */

    idx_t shift = n - 5;             /* which element of the sequence? */

    /* First call: initialize the buffer with the 12 known values. */
    if (built_up == (idx_t)(-1)) {
        for (int i = 0; i < 12; ++i) buffer[i] = init[i];
        built_up = 11;
    }

    if (shift <= built_up) {
        return buffer[shift % 12];
    }

    /* Compute terms up to the requested shift. */
    for (idx_t i = built_up + 1; i <= shift; ++i) {
        seq_t next = recurrence_step(i);
        buffer[i % 12] = next;
    }
    built_up = shift;
    return buffer[shift % 12];
}
