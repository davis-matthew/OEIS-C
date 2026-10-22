/*
 * A316691  Number of nX6 0..1 arrays with every element unequal to 0, 1, 2,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies the recurrence * a(n) = 2*a(n-1) +
 *            5*a(n-2) + 33*a(n-3) - 9*a(n-4) - 53*a(n-5) - 229*a(n-6) * +
 *            42*a(n-7) + 42*a(n-8) + 386*a(n-9) - 47*a(n-10) - 52*a(n-11) -
 *            288*a(n-12) * + 62*a(n-13) + 216*a(n-15) + 84*a(n-16) (n > 21) *
 *            with initial values * a(1)=32, a(2)=13, a(3)=42, a...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[23] = {
        0ULL,                /* unused 0 index */
        32ULL, 13ULL, 42ULL, 125ULL, 359ULL,
        2081ULL, 9442ULL, 38020ULL, 174483ULL, 785604ULL,
        3427907ULL, 15257880ULL, 67982637ULL, 300964560ULL,
        1335669886ULL, 5933105093ULL, 26325240756ULL,
        116828769988ULL, 518611346889ULL, 2301753155715ULL,
        10215798261389ULL, 45343030343793ULL
    };

    /* Return initial values directly */
    if (n <= 22) {
        return init[n];
    }

    /* Keep the last 16 terms: last[0] = a(n-1), .. , last[15] = a(n-16) */
    seq_t last[16];
    for (int k = 0; k < 16; ++k) {
        last[k] = init[22 - k];
    }

    seq_t val = 0;
    for (idx_t i = 23; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        val = 0;
        val += 2ULL * last[0];
        val += 5ULL * last[1];
        val += 33ULL * last[2];
        val += 42ULL * last[6];
        val += 42ULL * last[7];
        val += 386ULL * last[8];
        val += 62ULL * last[12];
        val += 216ULL * last[14];
        val += 84ULL * last[15];

        val -= 9ULL * last[3];
        val -= 53ULL * last[4];
        val -= 229ULL * last[5];
        val -= 47ULL * last[9];
        val -= 52ULL * last[10];
        val -= 288ULL * last[11];

        /* Shift array to prepare for next iteration */
        for (int k = 15; k > 0; --k) {
            last[k] = last[k - 1];
        }
        last[0] = val;
    }

    return val;
}
