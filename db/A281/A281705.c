/*
 * A281705  Number of Hangul letters (initials, medials and finals of
 *          syllables) in Native Korean name of n.
 * https://oeis.org/A281705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, more
 * Formula:   counts the number of Hangul jamos in the native Korean name of n
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t UNIT_COUNTS[10]      = {0,4,3,3,3,5,5,6,5,5};
static const seq_t MULT_COUNTS[10]      = {0,3,2,2,2,2,3,3,3,2};
static const seq_t TENS_COUNTS[10]     = {0,3,5,5,5,5,5,6,5,5};
static const seq_t GROUP_UNIT_COUNTS[5] = {0,3,3,2,3};

static seq_t count_group(idx_t g)
{
    seq_t cnt = 0;
    idx_t w = g / 1000;
    if (w > 0) {
        if (w == 1)
            cnt += 3;                      /* "천" */
        else
            cnt += MULT_COUNTS[w] + 3;     /* digit + "천" */
    }
    idx_t h = (g / 100) % 10;
    if (h > 0) {
        if (h == 1)
            cnt += 3;                      /* "백" */
        else
            cnt += MULT_COUNTS[h] + 3;     /* digit + "백" */
    }
    idx_t t = (g / 10) % 10;
    if (t > 0)
        cnt += TENS_COUNTS[t];             /* tens word */
    idx_t u = g % 10;
    if (u > 0)
        cnt += UNIT_COUNTS[u];             /* units word */
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety guard */

    idx_t groups[5];
    idx_t idx = 0;
    idx_t tmp = n;
    while (tmp > 0 && idx < 5) {
        groups[idx] = tmp % 10000;
        tmp /= 10000;
        ++idx;
    }
    idx_t max_idx = idx - 1;

    seq_t total = 0;
    for (idx_t i = 0; i <= max_idx; ++i) {
        idx_t g = groups[i];
        if (g == 0) continue;
        seq_t seg = count_group(g);
        if (i > 0) {
            if (i == max_idx && g == 1) {
                seg = GROUP_UNIT_COUNTS[i];           /* e.g., "만" */
            } else {
                seg += GROUP_UNIT_COUNTS[i];           /* digit + unit word */
            }
        }
        total += seg;
    }
    return total;
}
