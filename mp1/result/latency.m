figure
hold
x=1:10;
plot(x, rare, x, somewhat, '--', x, frequent, ':');
xlabel('id')
ylabel('latency (s)')
legend('rare','somewhat', 'frequent');
