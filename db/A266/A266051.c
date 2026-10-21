/*
 * A266051  Number of nX4 integer arrays with each element equal to the number
 *          of horizontal and antidiagonal neighbors less than or equal to
 *          itself.
 * https://oeis.org/A266051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A266051
   Recurrence (valid for n > 12):
   a(n) = 7*a(n-1) + 19*a(n-2) + 54*a(n-3) + 52*a(n-4)
          + 580*a(n-5) + 223*a(n-6) - 275*a(n-7) - 60*a(n-8)
          + 44*a(n-9) - 4*a(n-10)
   Initial terms (1‑based index):
   a(1)=7,  a(2)=58,  a(3)=302,  a(4)=4203,  a(5)=41919,
   a(6)=406412, a(7)=3922630, a(8)=37847050, a(9)=366073677,
   a(10)=3539729256, a(11)=34225117612, a(12)=330920692737
   (All terms used below fit into 64‑bit integers.)
 */
seq_t a(idx_t n)
{
    /* Initial 12 terms (index 0 unused) */
    static const seq_t init[13] = {
        0ULL,                             /* dummy for index 0 */
        7ULL,
        58ULL,
        302ULL,
        4203ULL,
        41919ULL,
        406412ULL,
        3922630ULL,
        37847050ULL,
        366073677ULL,
        3539729256ULL,
        34225117612ULL,
        330920692737ULL
    };

    if (n <= 12) {
        return init[n];
    }

    /* Coefficients of the recurrence */
    static const int coeff[10] = {
        7, 19, 54, 52, 580, 223, -275, -60, 44, -4
    };

    /* last[0] = a(k-1), last[1] = a(k-2), ..., last[9] = a(k-10) */
    seq_t last[10];
    last[0] = init[12];
    last[1] = init[11];
    last[2] = init[10];
    last[3] = init[ 9];
    last[4] = init[ 8];
    last[5] = init[ 7];
    last[6] = init[ 6];
    last[7] = init[ 5];
    last[8] = init[ 4];
    last[9] = init[ 3];

    for (idx_t k = 13; k <= n; ++k) {
        seq_t pos = 0;   /* sum of positive terms */
        seq_t neg = 0;   /* sum of absolute values of negative terms */

        for (int i = 0; i < 10; ++i) {
            int c = coeff[i];
            if (c > 0) {
                pos += (seq_t)c * last[i];
            } else if (c < 0) {
                neg += (seq_t)(-c) * last[i];
            }
            /* c == 0 has no effect */
        }

        seq_t next = pos - neg;   /* final value for a(k) */

        /* shift the window for the next iteration */
        for (int i = 9; i > 0; --i) {
            last[i] = last[i-1];
        }
        last[0] = next;
    }

    return last[0];   /* a(n) */
}
