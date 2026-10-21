/*
 * A269285  Number of n X 4 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          no more than once.
 * https://oeis.org/A269285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269285: Number of n X 4 0..3 arrays with a specific adjacency property
   implemented using the linear recurrence:
   a(1) = 216
   a(2) = 19188
   a(3) = 1363572
   a(n) = 98 * a(n-1) - 2401 * a(n-2)  for n > 3
*/

seq_t a(idx_t n) {
    /* Handle the initial conditions */
    if (n == 1) return (seq_t)216u;
    if (n == 2) return (seq_t)19188u;
    if (n == 3) return (seq_t)1363572u;

    /* Use the recurrence for n >= 4.
       a_nm2 holds a(n-2), a_nm1 holds a(n-1). */
    seq_t a_nm2 = (seq_t)19188u;   /* a(2) */
    seq_t a_nm1 = (seq_t)1363572u; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t temp = (seq_t)98u * a_nm1;     /* 98 * a(n-1) */
        seq_t sub  = (seq_t)2401u * a_nm2;   /* 2401 * a(n-2) */
        seq_t new_val = temp - sub;          /* a(n) = temp - sub  */

        /* Shift window for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }

    return a_nm1;
}
