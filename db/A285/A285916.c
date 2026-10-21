/*
 * A285916  Self-inverse permutation: in base 3, reverse the lengths of runs
 *          of consecutive equal digits.
 * https://oeis.org/A285916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is obtained by * 1. Writing n in base 3. * 2. Recording the
 *            lengths of consecutive equal digits (runs). * 3. Reversing this
 *            list of lengths. * 4. Assigning the reversed lengths to the runs
 *            in their original order. * 5. Re‑assembling the digits to form a
 *            base 3 number, which is a(n)
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Step 1: Convert n to base‑3 digits (least significant first). */
    unsigned char digits[64];
    int len = 0;
    seq_t temp = n;
    while (temp > 0) {
        digits[len++] = (unsigned char)(temp % 3);
        temp /= 3;
    }

    /* Step 2: Find runs: store digits of runs and their lengths. */
    unsigned char run_digits[64];
    int run_lens[64];
    int runs = 0;
    int i = len - 1;              /* index over digits most significant first */
    while (i >= 0) {
        unsigned char cur = digits[i];
        int cnt = 0;
        while (i >= 0 && digits[i] == cur) {
            cnt++;
            i--;
        }
        run_digits[runs] = cur;
        run_lens[runs]  = cnt;
        runs++;
    }

    /* Step 3 & 4: Build result digits by assigning reversed run lengths. */
    unsigned char result_digits[64];
    int res_len = 0;
    for (int j = 0; j < runs; ++j) {
        unsigned char digit = run_digits[j];
        int new_len = run_lens[runs - 1 - j];  /* reversed lengths */
        for (int k = 0; k < new_len; ++k) {
            result_digits[res_len++] = digit;
        }
    }

    /* Step 5: Convert result digits (most significant first) back to seq_t. */
    seq_t result = 0;
    for (int j = 0; j < res_len; ++j) {
        result = result * 3 + result_digits[j];
    }
    return result;
}
