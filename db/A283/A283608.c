/*
 * A283608  Numbers whose largest decimal digit is 5.
 * https://oeis.org/A283608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 4651, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose largest decimal digit is 5.
   Compute the nth qualifying integer by combinatorial digit construction. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAXLEN 20   /* enough for n up to at least 10^6 (length 6) */

static const idx_t pow6[MAXLEN + 1] = {
    1ULL,
    6ULL,
    36ULL,
    216ULL,
    1296ULL,
    7776ULL,
    46656ULL,
    279936ULL,
    1679616ULL,
    10077696ULL,
    60466176ULL,
    362797056ULL,
    2176782336ULL,
    13060694016ULL,
    78364164096ULL,
    470184984576ULL,
    2821109907456ULL,
    16926659444736ULL,
    101559956668416ULL,
    609359740010496ULL
};

static const idx_t pow5[MAXLEN + 1] = {
    1ULL,
    5ULL,
    25ULL,
    125ULL,
    625ULL,
    3125ULL,
    15625ULL,
    78125ULL,
    390625ULL,
    1953125ULL,
    9765625ULL,
    48828125ULL,
    244140625ULL,
    1220703125ULL,
    6103515625ULL,
    30517578125ULL,
    152587890625ULL,
    762939453125ULL,
    3814697265625ULL,
    19073486328125ULL
};

seq_t a(idx_t n)
{
    /* 1. find the number of digits L of the answer */
    idx_t cumulative = 0;
    idx_t L = 0;
    for (idx_t k = 1; k <= MAXLEN; ++k) {
        idx_t lenCount = 5 * pow6[k - 1] - 4 * pow5[k - 1];
        if (cumulative + lenCount >= n) {
            L = k;
            break;
        }
        cumulative += lenCount;
    }

    /* 2. position within the block of length L (1–indexed) */
    idx_t remaining = n - cumulative;

    /* 3. construct the number digit by digit */
    seq_t result = (seq_t)0;
    bool has5 = false;

    for (idx_t pos = 1; pos <= L; ++pos) {
        idx_t rem = L - pos;                      /* remaining free positions */
        int d_start = (pos == 1) ? 1 : 0;         /* first digit can't be 0 */

        for (int d = d_start; d <= 5; ++d) {
            bool new_has5 = has5 || (d == 5);
            idx_t count;
            if (new_has5) {
                count = pow6[rem];                /* any digits allowed */
            } else {
                count = pow6[rem] - pow5[rem];    /* must contain a 5 somewhere */
            }

            if (remaining > count) {
                remaining -= count;               /* skip this block */
            } else {
                result = result * 10 + (seq_t)d;   /* choose this digit */
                has5 = new_has5;
                break;                             /* move to next position */
            }
        }
    }

    return result;
}
