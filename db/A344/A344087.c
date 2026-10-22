/*
 * A344087  Flattened tetrangle of strict integer partitions sorted first by
 *          sum, then colexicographically.
 * https://oeis.org/A344087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the part value at index n in the flattened tetrangle of
 *            strict integer partitions sorted first by sum, then
 *            colexicographically
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const int MAX_PARTS = 400;   /* safe for n up to ~10000 */
static seq_t answer;
static idx_t target;
static idx_t counter;

/* Recursive generator of strict partitions of 'remaining'.
   Parts are built in ascending order in 'parts'.
   Returns 1 when the target index is reached, else 0. */
static int rec_generate(int remaining, int min_next, int *parts, int len) {
    if (remaining == 0) {
        for (int i = len - 1; i >= 0; --i) {   /* output parts descending */
            if (counter == target) {
                answer = (seq_t)parts[i];
                return 1;
            }
            ++counter;
        }
        return 0;
    }
    if (min_next > remaining)
        return 0;
    for (int part = min_next; part <= remaining; ++part) {
        if (len >= MAX_PARTS)
            break;
        parts[len] = part;
        if (rec_generate(remaining - part, part + 1, parts, len + 1))
            return 1;
    }
    return 0;
}

seq_t a(idx_t n) {
    target = n;
    counter = 0;
    int sum = 1;
    int partsArr[MAX_PARTS];
    while (1) {
        if (rec_generate(sum, 1, partsArr, 0))
            return answer;
        ++sum;
    }
}
