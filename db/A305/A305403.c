/*
 * A305403  Number of Ukrainian letters in Ukrainian name of n.
 * https://oeis.org/A305403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   number of Ukrainian letters in the Ukrainian name of n, counting
 *            letters in the word representation (apostrophes ignored)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int units[10] = {4,4,3,3,6,4,5,3,5,6};
static const int teens[10] = {6,10,10,10,12,10,11,10,12,12};
static const int tens[10]  = {0,0,8,8,5,8,9,8,10,9};
static const int hundreds[10] = {0,3,6,6,9,6,7,6,8,8};

static seq_t count_0_999(idx_t n)
{
    seq_t res = 0;
    if (n < 10)
        res = units[(int)n];
    else if (n < 20)
        res = teens[(int)(n - 10)];
    else if (n < 100) {
        int t = (int)(n / 10);
        int u = (int)(n % 10);
        res = tens[t];
        if (u > 0)
            res += units[u];
    } else {
        int h = (int)(n / 100);
        int rest = (int)(n % 100);
        res = hundreds[h];
        if (rest != 0) {
            if (rest < 10)
                res += units[rest];
            else if (rest < 20)
                res += teens[rest - 10];
            else {
                int t = rest / 10;
                int u = rest % 10;
                res += tens[t];
                if (u > 0)
                    res += units[u];
            }
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 4;               /* "нуль" */

    static const int suffix_len[10][3] = {
        {0,0,0},          /* unused */
        {6,6,5},          /* thousands: тисяча / тисячі / тисяч */
        {7,8,9},          /* millions: мільйон / мільйони / мільйонів */
        {7,8,9},          /* billions: мільярд / мільярди / мільярдів */
        {8,9,10},         /* trillions: трильйон / трильйони / трильйонів */
        {10,11,12},       /* quadrillions (approx.) */
        {12,13,14},       /* quintillions (approx.) */
        {14,15,16},       /* sextillions (approx.) */
        {16,17,18},       /* septillions (approx.) */
        {18,19,20}        /* octillions (approx.) */
    };

    seq_t res = 0;
    idx_t temp = n;
    int group = 0;

    while (temp > 0) {
        int g = (int)(temp % 1000);
        if (group == 0) {                 /* units */
            if (g != 0)
                res += count_0_999((idx_t)g);
        } else {                          /* thousands and higher */
            if (g != 0) {
                res += count_0_999((idx_t)g);
                int form;
                if (g == 1)
                    form = 0;
                else if (g % 100 >= 11 && g % 100 <= 14)
                    form = 2;
                else if (g % 10 == 1)
                    form = 0;
                else if (g % 10 >= 2 && g % 10 <= 4)
                    form = 1;
                else
                    form = 2;
                res += suffix_len[group][form];
            }
        }
        temp /= 1000;
        group++;
    }

    return res;
}
