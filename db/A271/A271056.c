/*
 * A271056  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 259", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (n>0?5:0) + 5*floor(n/2) + ( 8*T*(T+1)*(2*T+1)/3 +
 *            12*T*(T+1) - 4*T ), where T = (n>=3)?(n-1)/2:0
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Validated: standard, pass, 59 terms through n = 125 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sum_active_cells(idx_t n) {
    seq_t result = 1;                 /* a(0) = 1 */
    /* add a(1) if it exists */
    if (n >= 1) {
        result += 5;                  /* a(1) - a(0) = 5 */
    }

    /* even steps (k >= 2): 5 cells each */
    seq_t even_cnt = (seq_t)(n / 2);   /* floor(n/2) */
    result += 5 * even_cnt;

    /* odd steps (k >= 3): use the derived formula */
    seq_t T = 0;
    if (n >= 3) {
        T = (seq_t)((n - 1) / 2);     /* floor((n-1)/2) */
    }
    if (T > 0) {
        /* compute 8*T*(T+1)*(2*T+1)/3 safely */
        seq_t term1 = (seq_t)8 * T * (T + 1) * (2 * T + 1) / 3;
        seq_t term2 = (seq_t)12 * T * (T + 1);
        seq_t term3 = (seq_t)4 * T;
        result += term1 + term2 - term3;
    }
    return result;
}

seq_t a(idx_t n) {
    return sum_active_cells(n);
}
