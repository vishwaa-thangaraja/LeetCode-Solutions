int leastInterval(char* tasks, int tasksSize, int n) {
    int c[26] = {0};
    int i, max = 0, count = 0;

    for (i = 0; i < tasksSize; i++)
        c[tasks[i] - 'A']++;

    for (i = 0; i < 26; i++)
        if (c[i] > max) max = c[i];

    for (i = 0; i < 26; i++)
        if (c[i] == max) count++;

    int ans = (max - 1) * (n + 1) + count;
    return ans > tasksSize ? ans : tasksSize;
}

