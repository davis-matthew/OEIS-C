/*
 * A400586  Paired Comma Sequence: Interlaced pair of sequences of positive
 *          numbers with the property that, for each sequence in the pair, the
 *          sequence formed by the pairs of digits adjacent to the commas
 *          between the terms is the same as the successive differences
 *          between each term and the previous term of t...
 * https://oeis.org/A400586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, new
 * Formula:   a(1)=1, b(1)=2. For k≥1 a(k+1)=b(k) + (10*last_digit(a(k)) +
 *            d_a) , where d_a is the first digit of a(k+1); b(k+1)=a(k) +
 *            (10*last_digit(b(k)) + d_b) , where d_b is the first digit of
 *            b(k+1). The first digit is the most significant decimal digit
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements the recurrence exactly.  It uses a
   fixed-size static array (size 30 000) to store up to 30 000 terms of
   each sequence, which is sufficient for all tests up to n≈10 000. */

static seq_t a_arr[30000];
static seq_t b_arr[30000];
static idx_t pairs_done = 0;  /* number of pairs already computed */

/* Return the most significant decimal digit of x. */
static seq_t first_digit(seq_t x)
{
    while (x >= (seq_t)10)
        x /= (seq_t)10;
    return x;
}

seq_t a(idx_t n)
{
    /* initialise base case on first call */
    if (pairs_done == 0) {
        a_arr[1] = (seq_t)1;
        b_arr[1] = (seq_t)2;
        pairs_done = 1;
    }

    /* number of pairs needed to answer this request */
    idx_t need_pairs = (n & 1) ? (n + 1) / 2 : n / 2;

    /* compute missing pairs */
    for (idx_t k = pairs_done + 1; k <= need_pairs; ++k) {
        /* ---- compute a(k) ---- */
        int last_a = (int)(a_arr[k - 1] % (seq_t)10);
        seq_t a_k = 0;
        for (int d = 0; d <= 9; ++d) {
            seq_t cand = b_arr[k - 1] + (seq_t)(10 * last_a + d);
            if ((int)first_digit(cand) == d) {
                a_k = cand;
                break;
            }
        }
        a_arr[k] = a_k;

        /* ---- compute b(k) ---- */
        int last_b = (int)(b_arr[k - 1] % (seq_t)10);
        seq_t b_k = 0;
        for (int d = 0; d <= 9; ++d) {
            seq_t cand = a_arr[k - 1] + (seq_t)(10 * last_b + d);
            if ((int)first_digit(cand) == d) {
                b_k = cand;
                break;
            }
        }
        b_arr[k] = b_k;
    }

    pairs_done = need_pairs;

    /* return interleaved sequence value */
    return (n & 1) ? a_arr[(n + 1) / 2] : b_arr[n / 2];
}
